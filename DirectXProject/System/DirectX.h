#ifndef __DIRECT_X_H__
#define __DIRECT_X_H__

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}

enum BlendMode
{
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_MAX
};
enum CullingMode
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	CULL_MAX
};

enum RenderTargetKind
{
	RT_BACKBUFFER,
	RT_RENDERTEX0,
	RT_RENDERTEX1,
};

HRESULT InitDX(HWND hWnd, UINT width, UINT height, bool fullscreen);
void UninitDX();
void BeginDrawDX();
void EndDrawDX();

ID3D11Device *GetDevice();
ID3D11DeviceContext *GetContext();
ID3D11DepthStencilView* GetDepthStencil();
void SetBlendMode(BlendMode blend);
void SetCulling(CullingMode cull);
void SetZBuffer(bool enable);
#endif // __DIRECT_X_H__