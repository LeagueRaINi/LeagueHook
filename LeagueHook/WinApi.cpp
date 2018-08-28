#include "WinApi.h"

PPEB WinApi::_peb = nullptr;

auto WinApi::GetPEB() -> PPEB
{
#if	defined(_WIN64)
	return reinterpret_cast< PTEB >( __readgsqword( 0x30 ) )->ProcessEnvironmentBlock;
#else
	return reinterpret_cast< PPEB >( __readfsdword( 0x30 ) );
#endif
}

auto WinApi::GetModuleInfo( fnv::hash module_name_hash ) -> PLDR_MODULE
{
	if ( _peb == nullptr &&  (_peb = GetPEB() ) == nullptr )
		return NULL;

	auto ldr = _peb->Ldr;
	auto module = PLDR_MODULE( ldr->Reserved2[ 1 ] );

	while ( module->BaseAddress != nullptr )
	{
		std::wstring base_name = module->BaseDllName.Buffer;
		std::transform( base_name.begin(), base_name.end(), base_name.begin(), ::tolower );

		if ( fnv::hash_runtime< const wchar_t* >( base_name.c_str() ) == module_name_hash )
			return module;

		module = PLDR_MODULE( module->InLoadOrderModuleList.Flink );
	}

	return NULL;
}

auto WinApi::GetProcAddress(ULONG64 module_base, fnv::hash proc_name_hash) -> std::uintptr_t
{
	const auto dos_header = PIMAGE_DOS_HEADER( module_base );
	const auto nt_header = PIMAGE_NT_HEADERS( PBYTE( dos_header ) + dos_header->e_lfanew );
	const auto opt_header = &nt_header->OptionalHeader;
	const auto exp_entry = PIMAGE_DATA_DIRECTORY( &opt_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ] );
	const auto virtual_address = exp_entry->VirtualAddress;
	const auto exp_dir = PIMAGE_EXPORT_DIRECTORY( module_base + virtual_address );

	for ( unsigned int i = 0; i < exp_dir->NumberOfNames; i++ )
	{
		const auto address_of_names = PULONG( module_base + exp_dir->AddressOfNames + ( sizeof( ULONG ) * i) );
		const auto function_name = ( char* )( module_base + *address_of_names );

		if ( !function_name )
			continue;
		if ( fnv::hash_runtime< const char* >( function_name ) != proc_name_hash )
			continue;

		const auto address_of_name_ordinals = *( PWORD )( module_base + ( exp_dir->AddressOfNameOrdinals + ( i * sizeof( WORD ) ) ) );
		const auto address_of_functions = *PULONG( module_base + ( exp_dir->AddressOfFunctions + ( sizeof( ULONG ) * address_of_name_ordinals ) ) );

		// check if the func is forwarded
		if ( address_of_functions > virtual_address && address_of_functions < virtual_address + exp_entry->Size )
		{
			const auto stringLength = [](const char* str, const char end_char) -> int
			{
				auto length = 0;
				while (str[length] != end_char)
					length++;

				return length;
			};

			const auto forward_str = ( char* )( module_base + address_of_functions );
			const auto str_length = stringLength( forward_str, '\0' );
			const auto dll_length = stringLength( forward_str, '.' );

			auto dll_name = ( char* )malloc( sizeof( char ) * ( dll_length + 5 ) );
			auto* func_name = ( char* )malloc( sizeof( char ) * ( str_length - ( dll_length - 1 ) ) );

			for ( auto j = 0; j < str_length; j++ )
				if ( j < dll_length )
					dll_name[ j ] = tolower( forward_str[ j ] );
				else
					func_name[ j - dll_length ] = forward_str[ j + 1 ];

			dll_name[ dll_length + 0 ] = '.';
			dll_name[ dll_length + 1 ] = 'd';
			dll_name[ dll_length + 2 ] = 'l';
			dll_name[ dll_length + 3 ] = 'l';
			dll_name[ dll_length + 4 ] = '\0';

			func_name[ str_length - dll_length - 1 ] = '\0';

			const auto new_module_info = GetModuleInfo( fnv::hash_runtime< char* >( dll_name ) );
			const auto new_module_base = ( ULONG64 )( new_module_info->BaseAddress == nullptr ? LI( LoadLibraryA )( dll_name ) : new_module_info->BaseAddress );
			const auto new_proc_address = GetProcAddress( new_module_base, fnv::hash_runtime< char* >( func_name ) );

			free( dll_name );
			free( func_name );

			return new_proc_address;
		}

		return std::uintptr_t( module_base + address_of_functions );
	}

	return NULL;
}

auto WinApi::GetProcAddressDynamic( fnv::hash proc_name_hash ) -> std::uintptr_t
{
	if ( _peb == nullptr && ( _peb = GetPEB() ) == nullptr )
		return NULL;

	auto ldr = _peb->Ldr;
	auto module = PLDR_MODULE( ldr->Reserved2[ 1 ] );
	auto module_base = module->BaseAddress;

	while ( module_base != nullptr )
	{
		auto proc_address = GetProcAddress( ( ULONG64 )module_base, proc_name_hash );
		if ( proc_address )
			return proc_address;

		module = PLDR_MODULE( module->InLoadOrderModuleList.Flink );
		module_base = module->BaseAddress;
	}

	return NULL;
}
