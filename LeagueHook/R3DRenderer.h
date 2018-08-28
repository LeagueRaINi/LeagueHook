#pragma once

#include "DeviceHandler.h"

#include <D3dx9math.h>

class R3DRenderer
{
public:
	char pad_0000[20]; //0x0000
	DeviceHandler* DeviceHandler; //0x0014
	char pad_0018[28]; //0x0018
	int ScreenResolutionX; //0x0034
	int ScreenResolutionY; //0x0038
	char pad_003C[108]; //0x003C
	D3DXMATRIX ViewMatrix; //0x00A8
	D3DXMATRIX ProjectionMatrix; //0x00E8
};