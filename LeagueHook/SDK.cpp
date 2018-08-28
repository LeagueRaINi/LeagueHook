#include "SDK.h"
#include "Globals.h"
#include "Utilities.h"
#include "WinApi.h"

R3DRenderer* g_Renderer = nullptr;
GameStateInstance* g_GameStateInstance = nullptr;

auto SDK::Init() -> void
{
	Globals::Log->LogInfo( "setting up sdk..." );

	auto main_module = Utils::GetModule( LI(GetCurrentProcessId)() , FNV( "League of Legends.exe" ) );
	if ( main_module.modBaseAddr == nullptr )
	{
		Globals::Log->LogError( "SDK::Init() failed. could not get main module" );
		return;
	}

	g_Renderer = **reinterpret_cast< R3DRenderer*** >( Utils::FindPattern( main_module, "A1 ? ? ? ? 83 B8 ? ? ? ? ? 75 50" ) + 0x1 );
	g_GameStateInstance = *reinterpret_cast< GameStateInstance** >( Utils::FindPattern( main_module, "83 3D ? ? ? ? ? 75 07 E8 ? ? ? ? 59 C3" ) + 0x2 );

#if _DEBUG
	#define STRINGIFY_IMPL( s ) #s
	#define STRINGIFY( s ) STRINGIFY_IMPL( s )
	#define PRINT_INTERFACE( name ) Globals::Log->LogDebug( "interface: %s - 0x%p", STRINGIFY( name ), name )

	PRINT_INTERFACE( g_Renderer );
	PRINT_INTERFACE( g_GameStateInstance );
#endif
}