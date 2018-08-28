#pragma once

#include "FNV-1A.h"

#include <windows.h>
#include <TlHelp32.h>
#include <string>

class Utils
{
public:
	static auto GetEpochTime() -> std::int64_t;
	static auto GetTimeStamp( bool only_time, bool path_friendly = false ) -> const std::string;

	static auto GetCurrentDir( char* path ) -> bool;
	static auto CreateFolder( char* path, const char* name ) -> bool;

	static auto GetModule( DWORD proc_id, fnv::hash hashed_mod_name ) -> MODULEENTRY32W;
	static auto FindPattern( MODULEENTRY32W module_info, const char* pattern ) -> std::uintptr_t;
};