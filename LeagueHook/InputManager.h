#pragma once

#include <cstdint>
#include <Windows.h>
#include <functional>
#include <d3d9.h>

enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed /*Down and then up*/
};

DEFINE_ENUM_FLAG_OPERATORS(KeyState)

class InputManager
{
public:
	InputManager( IDirect3DDevice9* device );
	~InputManager();

	auto GetMainWindow() -> HWND;
	auto GetOldWndProc() -> LONG_PTR;

	auto GetKeyState( std::uint32_t vk ) -> KeyState;
	auto IsKeyDown( std::uint32_t vk ) -> bool;
	auto WasKeyPressed( std::uint32_t vk ) -> bool;

	auto RegisterHotkey( std::uint32_t vk, std::function< void() > f) -> void;
	auto RemoveHotkey( std::uint32_t vk ) -> void;

private:
	HWND _target_window;
	LONG_PTR _old_wnd_proc;
	KeyState _key_map[ 256 ] {};
	std::function<void()> _hotkeys[ 256 ];

	static auto WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) -> LRESULT;

	auto ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool;
	auto ProcessMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool;
	auto ProcessKeybdMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) -> bool;
};