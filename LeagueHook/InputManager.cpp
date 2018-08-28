#include "InputManager.h"
#include "Globals.h"
#include "WinApi.h"

#include "ImGui/imgui.h"

// TODO: dirty....
LRESULT ImGui_ImplDX9_WndProcHandler( HWND, UINT msg, WPARAM wParam, LPARAM lParam )
{
	auto& io = ImGui::GetIO();

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[ 0 ] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[ 0 ] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[ 1 ] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[ 1 ] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[ 2 ] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[ 2 ] = false;
		return true;
	case WM_XBUTTONDOWN:
		if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON1 ) == MK_XBUTTON1 )
			io.MouseDown[ 3 ] = true;
		else if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON2 ) == MK_XBUTTON2 )
			io.MouseDown[ 4 ] = true;
		return true;
	case WM_XBUTTONUP:
		if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON1 ) == MK_XBUTTON1 )
			io.MouseDown[ 3 ] = false;
		else if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON2 ) == MK_XBUTTON2 )
			io.MouseDown[ 4 ] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = static_cast< signed short >( lParam );
		io.MousePos.y = static_cast< signed short >( lParam >> 16 );
		return true;
	case WM_KEYDOWN:
		if ( wParam < 256 )
			io.KeysDown[wParam] = true;
		return true;
	case WM_KEYUP:
		if ( wParam < 256 )
			io.KeysDown[ wParam ] = false;
		return true;
	case WM_CHAR:
		if ( wParam > 0 && wParam < 0x10000 )
			io.AddInputCharacter( static_cast< unsigned short >( wParam ) );
		return true;
	default:
		break;
	}

	return NULL;
}

InputManager::InputManager( IDirect3DDevice9* device )
{
	Globals::Log->LogInfo( "setting up input manager..." );

	D3DDEVICE_CREATION_PARAMETERS params;
	if ( FAILED( device->GetCreationParameters( &params ) ) )
	{
		Globals::Log->LogError( "could not setup input manager. GetCreationParameters failed" );
		return;
	}

	_target_window = params.hFocusWindow;
	_old_wnd_proc = SetWindowLongPtr( _target_window, GWLP_WNDPROC, LONG_PTR( WndProc ) );

	if ( _old_wnd_proc )
		Globals::Log->LogDebug( "hooked WndProc" );
	else
		Globals::Log->LogError( "could not setup input manager. SetWindowLongPtr failed" );
}

InputManager::~InputManager()
{
	if ( _old_wnd_proc )
		SetWindowLongPtr( _target_window, GWLP_WNDPROC, _old_wnd_proc );

	_old_wnd_proc = 0;
	_target_window = NULL;
}

auto InputManager::GetMainWindow() -> HWND
{
	return _target_window;
}

auto InputManager::GetOldWndProc() -> LONG_PTR
{
	return _old_wnd_proc;
}

auto InputManager::GetKeyState( std::uint32_t vk ) -> KeyState
{
	return _key_map[ vk ];
}

auto InputManager::IsKeyDown( std::uint32_t vk ) -> bool
{
	return _key_map[ vk ] == KeyState::Down;
}

auto InputManager::WasKeyPressed( std::uint32_t vk ) -> bool
{
	if ( _key_map[ vk ] == KeyState::Pressed )
	{
		_key_map[ vk ] = KeyState::Up;
		return true;
	}

	return false;
}

auto InputManager::RegisterHotkey( std::uint32_t vk, std::function< void() > f ) -> void
{
	_hotkeys[ vk ] = f;
}

auto InputManager::RemoveHotkey( std::uint32_t vk ) -> void
{
	_hotkeys[ vk ] = nullptr;
}

auto WINAPI InputManager::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) -> LRESULT
{
	Globals::Input->ProcessMessage( msg, wParam, lParam );

	// TODO:
	ImGui_ImplDX9_WndProcHandler( hWnd, msg, wParam, lParam );

	return LI( CallWindowProcW )( WNDPROC( Globals::Input->GetOldWndProc() ), hWnd, msg, wParam, lParam );
}

auto InputManager::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool
{
	switch ( uMsg )
	{
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_XBUTTONUP:
		return ProcessMouseMessage( uMsg, wParam, lParam );
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return ProcessKeybdMessage( uMsg, wParam, lParam );
	default:
		return false;
	}
}

auto InputManager::ProcessMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool
{
	auto key = VK_LBUTTON;
	auto state = KeyState::None;

	switch ( uMsg )
	{
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_MBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_RBUTTON;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_LBUTTON;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
		key = ( HIWORD( wParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
		break;
	default:
		return false;
	}

	_key_map[ key ] = state == KeyState::Up && _key_map[ key ] == KeyState::Down ? KeyState::Pressed : state;

	return true;
}

auto InputManager::ProcessKeybdMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool
{
	const auto key = wParam;
	auto state = KeyState::None;

	switch ( uMsg )
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KeyState::Up;
		break;
	default:
		return false;
	}

	if (state == KeyState::Up && _key_map[ int( key ) ] == KeyState::Down)
	{
		_key_map[ int( key ) ] = KeyState::Pressed;

		auto& hotkey_callback = _hotkeys[ key ];
		if ( hotkey_callback )
			hotkey_callback();
	}
	else
		_key_map[ int( key ) ] = state;

	return true;
}
