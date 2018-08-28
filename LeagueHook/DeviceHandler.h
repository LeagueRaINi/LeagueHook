#pragma once

#include <d3d9.h>

class DeviceHandler
{
public:
	char pad_0000[20];					//0x0000
	LPDIRECT3DDEVICE9 Direct3DDevice9;	//0x0014
};