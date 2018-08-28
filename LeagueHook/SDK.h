#pragma once

#include "R3DRenderer.h"
#include "GameStateInstance.h"

extern R3DRenderer* g_Renderer;
extern GameStateInstance* g_GameStateInstance;

class SDK
{
public:
	static auto Init() -> void;
};