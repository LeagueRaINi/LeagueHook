#include "Math.h"
#include "SDK.h"

auto Math::WorldToScreen( D3DXVECTOR3 world_pos, D3DXVECTOR3* screen_pos ) -> bool
{
	if ( g_Renderer == nullptr )
		return false;

	auto device_handler = g_Renderer->DeviceHandler;
	if ( device_handler == nullptr )
		return false;

	auto device = device_handler->Direct3DDevice9;
	if ( device == nullptr )
		return false;

	D3DVIEWPORT9 viewport;
	device->GetViewport( &viewport );

	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );

	D3DXVec3Project( screen_pos, &world_pos, &viewport, &g_Renderer->ProjectionMatrix, &g_Renderer->ViewMatrix, &identity );

	return true;
}