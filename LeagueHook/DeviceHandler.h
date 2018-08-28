#pragma once

#include <d3d9.h>

class DeviceHandler;
class DeviceHandlerVTable
{
	typedef int(__thiscall* fnD3D9Function)(DeviceHandler *);
public:
	char pad_0000[40]; //0x0000
	fnD3D9Function mBeginScene; //0x0028
	fnD3D9Function mEndScene; //0x002C
};

class DeviceHandler
{
public:
	DeviceHandlerVTable* VTable;
	char pad_0004[16]; //0x0004
	LPDIRECT3DDEVICE9 Direct3DDevice9; //0x0014
};