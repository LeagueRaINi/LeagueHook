#pragma once

#include "GameObject.h"

#include <cstdint>

class GameStateInstance
{
public:
	char pad_0000[8];				//0x0000
	std::int32_t GameState;			//0x0008
	char pad_000C[88];				//0x000C
	GameObject* HoveredObject;		//0x0064
	char pad_0068[36];				//0x0068
	float GameTime;					//0x008C
	std::int32_t MousePositionX;	//0x0090
	std::int32_t MousePositionY;	//0x0094
};