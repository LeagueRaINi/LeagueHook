#include "Math.h"
#include "SDK.h"

auto Math::WorldToScreen(D3DXVECTOR3 world_pos) -> D3DXVECTOR2
{
	const auto viewMatrix = &g_Renderer->ViewMatrix;
	const auto projection_matrix = &g_Renderer->ProjectionMatrix;

	auto tmp = &D3DXVECTOR4(world_pos.x, world_pos.y, world_pos.z, 1);

	D3DXVec4Transform(tmp, tmp, viewMatrix);
	D3DXVec4Transform(tmp, tmp, projection_matrix);

	tmp->y *= -1;

	tmp->x /= tmp->w;
	tmp->y /= tmp->w;

	tmp->x += 1;
	tmp->y += 1;

	tmp->x *= (float)g_Renderer->ScreenResolutionX / 2;
	tmp->y *= (float)g_Renderer->ScreenResolutionY / 2;

	return {tmp->x, tmp->y};
}