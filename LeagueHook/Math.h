#pragma once

#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib") // move later

class Math
{
public:
	static auto WorldToScreen( D3DXVECTOR3 world_pos, D3DXVECTOR3* screen_pos ) -> bool;
};