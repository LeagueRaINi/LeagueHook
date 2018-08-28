#pragma once

#include "FNV-1A.h"

#include <iostream>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <winternl.h>
#include <intrin.h>

#define toStr( x ) #x

#define LI_FUNCTION1( fn ) WinApi::GetProcAddressLazy< decltype( &fn ), FNV( toStr( fn ) ), 0 >()
#define LI_FUNCTION2( mod, fn ) WinApi::GetProcAddressLazy< decltype( &fn ), FNV( toStr( fn ) ), FNV( mod ) >()

#define LI_GET_MACRO( _1, _2, NAME ,... ) NAME
#define LI_EXPAND( x ) x
#define LI( ... ) LI_EXPAND( LI_GET_MACRO( __VA_ARGS__, LI_FUNCTION2, LI_FUNCTION1 )( __VA_ARGS__ ) )

class WinApi
{
public:
	typedef struct _LDR_MODULE
	{
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
		PVOID BaseAddress;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		SHORT LoadCount;
		SHORT TlsIndex;
		LIST_ENTRY HashTableEntry;
		ULONG TimeDateStamp;
	} LDR_MODULE, *PLDR_MODULE;

	static auto GetPEB() -> PPEB;
	static auto GetModuleInfo( fnv::hash module_name_hash ) -> PLDR_MODULE;
	static auto GetProcAddress( ULONG64 module_base, fnv::hash proc_name_hash ) -> std::uintptr_t;
	static auto GetProcAddressDynamic( fnv::hash proc_name_hash ) -> std::uintptr_t;

	template < fnv::hash ModuleNameHash >
	struct ModuleCache
	{
	public:
		static auto get_module() -> PLDR_MODULE
		{
			static PLDR_MODULE module_info{};

			if ( module_info == nullptr || module_info->BaseAddress == nullptr )
				module_info = GetModuleInfo( ModuleNameHash );

			return module_info;
		}
	};

	template < fnv::hash ProcNameHash, fnv::hash ModuleNameHash >
	struct ProcAddressCache : ModuleCache< ModuleNameHash >
	{
	public:
		static auto get_proc_address() -> std::uintptr_t
		{
			static std::uintptr_t function = 0;

			if ( function == 0 )
			{
				auto module_info = ProcAddressCache::get_module();
				if ( module_info == nullptr || module_info->BaseAddress == nullptr )
					return function;

				function = GetProcAddress( ( ULONG64 )module_info->BaseAddress, ProcNameHash );
			}

			return function;
		}
	};

	template < fnv::hash ProcNameHash >
	struct ProcAddressCache< ProcNameHash, 0 >
	{
		static auto get_proc_address() -> std::uintptr_t
		{
			static std::uintptr_t function = 0;

			if ( function == 0 )
				function = GetProcAddressDynamic( ProcNameHash );

			return function;
		}
	};

	template < typename DeclType, fnv::hash ProcNameHash, fnv::hash ModuleNameHash >
	static auto GetProcAddressLazy() -> DeclType
	{
		return reinterpret_cast< DeclType >( ProcAddressCache< ProcNameHash, ModuleNameHash >::get_proc_address() );
	}

private:
	static PPEB _peb;
};