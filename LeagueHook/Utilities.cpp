#include "Utilities.h"
#include "Globals.h"
#include "WinApi.h"

#include <chrono>

#define INRANGE( x, a, b ) ( x >= a && x <= b )
#define GETBITS( x ) ( INRANGE( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( INRANGE( x, '0', '9' ) ? x - '0' : 0 ) )
#define GETBYTE( x ) ( GETBITS( x[ 0 ] ) << 4 | GETBITS( x[ 1 ] ) )

auto Utils::GetEpochTime() -> std::int64_t
{
	return std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() ).count();
}

auto Utils::GetTimeStamp( bool only_time, bool path_friendly ) -> const std::string
{
	auto date = tm{};
	auto c_time = time( nullptr );

	localtime_s( &date, &c_time );

	char buf[ 30 ];
	strftime( buf, sizeof( buf ), path_friendly
		? only_time
			? "%H-%M-%S"
			: "%Y-%m-%d.%H-%M-%S"
		: only_time
			? "%H:%M:%S"
			: "%Y.%m.%d - %H:%M:%S", &date );

	return std::string(buf);
}

auto Utils::GetCurrentDir( char* path ) -> bool
{
	if ( LI( GetCurrentDirectoryA )( MAX_PATH, path ) < 1 )
	{
		Globals::Log->LogError( "Utils::GetCurrentDir() failed" );
		return false;
	}

	return true;
}

auto Utils::CreateFolder( char* path, const char* name ) -> bool
{
	strcat_s( path, MAX_PATH, name );

	if ( LI( CreateDirectoryA )( path, nullptr ) == 0 && LI( GetLastError )() == ERROR_PATH_NOT_FOUND )
	{
		Globals::Log->LogError( "Utils::CreateFolder() failed. invalid path" );
		return false;
	}

	return true;
}

auto Utils::GetModule( DWORD proc_id, fnv::hash hashed_mod_name ) -> MODULEENTRY32W
{
	auto snapshot = LI( CreateToolhelp32Snapshot )( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id );
	if ( snapshot == INVALID_HANDLE_VALUE )
		return MODULEENTRY32W{};

	auto snapshot_close = std::unique_ptr< std::remove_pointer< HANDLE >::type, BOOL( WINAPI* )( HANDLE ) >( snapshot, LI( CloseHandle ) );

	MODULEENTRY32W entry = { sizeof( MODULEENTRY32W ) };

	if ( LI( Module32FirstW )( snapshot, &entry ) )
	{
		do
		{
			if ( fnv::hash_runtime< const wchar_t* >( entry.szModule ) == hashed_mod_name )
				return entry;

		} while ( LI( Module32NextW )(snapshot, &entry ) );
	}

	return MODULEENTRY32W{};
}

auto Utils::FindPattern( MODULEENTRY32W module_info, const char* pattern ) -> std::uintptr_t
{
	const auto start_address = std::uintptr_t( module_info.modBaseAddr );
	const std::uintptr_t end_address = start_address + module_info.modBaseSize;

	auto temp_pattern = pattern;
	std::uintptr_t first_match = 0;

	for ( auto position = start_address; position < end_address; position++ )
	{
		if ( !*temp_pattern )
			return first_match;

		const auto pattern_current = *reinterpret_cast< const std::uint8_t* >( temp_pattern );
		const auto memory_current = *reinterpret_cast< const std::uint8_t* >( position );

		if ( pattern_current == '\?' || memory_current == GETBYTE( temp_pattern ) )
		{
			if ( !first_match )
				first_match = position;
			if ( !temp_pattern[ 2 ] )
				return first_match;

			temp_pattern += pattern_current != '\?' ? 3 : 2;
		}
		else
		{
			temp_pattern = pattern;
			first_match = 0;
		}
	}

	return NULL;
}