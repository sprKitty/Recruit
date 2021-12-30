#pragma once

#include <d3d11.h>
#include <System/ClassDesign/Singleton.h>
#include <memory>
#include <vector>
#include <Vector.h>

#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}

namespace BlendMode
{
	enum Kind
	{
		BLEND_ALPHA,
		BLEND_ADD,
		BLEND_MAX
	};
};


namespace CullingMode
{
	enum Kind
	{
		CULL_NONE,
		CULL_FRONT,
		CULL_BACK,
		CULL_MAX
	};
};

namespace DrawPass
{
	enum Kind
	{
		BACKBUFFER,
		MULTIPATH,

		MAX
	};
}

class DepthStencil;

class DirectX11 : public Singleton<DirectX11>
{
public:
	friend class Singleton<DirectX11>;

public:
	void Initialize()override {}
	void Finalize()override;

	const HRESULT Start(HWND hWnd, UINT width, UINT height, bool fullscreen);
	void BeginDraw(D3D11_VIEWPORT* vp = nullptr, ID3D11RenderTargetView** pRTV = nullptr, ID3D11DepthStencilView* pDSV = nullptr, const std::vector<Vector4>* clearList = nullptr, const int num = 1);
	void EndDraw();

	void SetBlendMode(const BlendMode::Kind kind);
	void SetCulling(CullingMode::Kind kind);

	ID3D11Device* GetDevice()
	{
		return m_pDevice;
	}

	ID3D11DeviceContext* GetContext()
	{
		return m_pContext;
	}

protected:
	DirectX11() {}
	~DirectX11()override {}

private:
	float m_fWidth;
	float m_fHeight;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain; 
	ID3D11RenderTargetView* m_pBBRT;
	ID3D11DepthStencilView*	m_pDepthStencil;
	ID3D11BlendState* m_pBlendState[BlendMode::BLEND_MAX];
	ID3D11RasterizerState* m_pRasterizer[CullingMode::CULL_MAX];
	D3D11_VIEWPORT m_vp;
};