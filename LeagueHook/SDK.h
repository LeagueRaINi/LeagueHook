#pragma once

#include "R3DRenderer.h"

extern R3DRenderer* g_Renderer;

class SDK
{
public:
	static auto Init() -> void;
};