#include "Hooks.h"
#include "SDK.h"
#include "VMTHook.h"
#include "Globals.h"
#include "Math.h"

#include "ImGui/imgui.h"
#include "ImGui/Directx9/imgui_impl_dx9.h"
#include "ImGui/imgui_internal.h"

std::unique_ptr<VMTHook> _d3d9_hook;

static HRESULT __stdcall Reset( IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params );
static HRESULT __stdcall Present( IDirect3DDevice9* thisptr, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region );

typedef long( __stdcall* Reset_t ) ( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
typedef long( __stdcall* Present_t ) ( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );

auto Hooks::Init() -> void
{
	Globals::Log->LogInfo( "setting up hooks..." );

	_d3d9_hook = std::make_unique< VMTHook >( g_Renderer->DeviceHandler->Direct3DDevice9 );

	if ( SUCCEEDED( _d3d9_hook->Hook( 17, Present ) ) )
		Globals::Log->LogDebug( "hooked present" );
	else
		Globals::Log->LogError( "failed to hook present" );

	if ( SUCCEEDED( _d3d9_hook->Hook( 16, Reset ) ) )
		Globals::Log->LogDebug("hooked reset");
	else
		Globals::Log->LogError( "failed to hook reset" );
}

auto Hooks::Restore() -> void
{
	if ( _d3d9_hook != nullptr )
	{
		_d3d9_hook->Unhook( 17 );
		_d3d9_hook->Unhook( 16 );
	}
}

HRESULT STDMETHODCALLTYPE Present( IDirect3DDevice9* thisptr, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region )
{
	const auto old_present = _d3d9_hook->GetOriginal< Present_t >( 17 );

	static auto init = false;

	if ( !init && thisptr )
	{
		ImGui_ImplDX9_Init( Globals::Input->GetMainWindow(), thisptr );

		init = true;
	}
	else if ( init && thisptr )
	{
		ImGui_ImplDX9_NewFrame();

		static auto draw_hovered_obj = false;
		static auto draw_hovered_obj_name = false;

		if ( Globals::ShowMenu )
		{
			ImGui::SetNextWindowSize( ImVec2( 600, 350 ), ImGuiCond_Once );
			ImGui::SetNextWindowPosCenter( ImGuiCond_Once );

			if ( ImGui::Begin( "Hello from ImGui!", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse ) )
			{
				ImGui::Checkbox( "Draw Hovered Object", &draw_hovered_obj );

				if ( draw_hovered_obj )
					ImGui::Checkbox( "- Name", &draw_hovered_obj_name);

				ImGui::End();
			}
		}

		ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f ) );

		if ( ImGui::Begin( "##overlay", nullptr, ImVec2( 0.0f, 0.0f ), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs ) )
		{
			ImGui::SetWindowPos( ImVec2( 0.0f, 0.0f ), ImGuiSetCond_Always );
			ImGui::SetWindowSize( ImVec2( ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y ), ImGuiSetCond_Always );

			auto draw_list = ImGui::GetCurrentWindow()->DrawList;

			if ( draw_hovered_obj )
			{
				auto hovered_obj = g_GameStateInstance->HoveredObject;
				if ( hovered_obj != nullptr )
				{
					auto cursor_w2s = Math::WorldToScreen( hovered_obj->WorldPosition );

					draw_list->AddCircle( ImVec2( cursor_w2s.x, cursor_w2s.y ), 25, ImColor( 255, 5, 100, 255 ), 100, 2 );

					if ( draw_hovered_obj_name )
						draw_list->AddText( ImVec2( cursor_w2s.x + 35, cursor_w2s.y - 5 ), ImColor( 255, 255, 100, 255 ), hovered_obj->ObjectName );
				}
			}

			draw_list->PushClipRectFullScreen();

			ImGui::End();
		}

		ImGui::PopStyleColor();

		ImGui::Render();
	}

	return old_present( thisptr, src, dest, wnd_override, dirty_region );
}

HRESULT STDMETHODCALLTYPE Reset( IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params )
{
	const auto old_reset = _d3d9_hook->GetOriginal< Reset_t >( 16 );

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = old_reset( thisptr, params );
	if ( SUCCEEDED(hr) )
		ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}