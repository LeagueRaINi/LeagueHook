#pragma once

#include <d3dx9math.h>

class GameObject
{
public:
	char pad_0000[36];				//0x0000
	char* ObjectName;				//0x0024
	char pad_0028[56];				//0x0028
	D3DXVECTOR3 WorldPosition;		//0x0060
};