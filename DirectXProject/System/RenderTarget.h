#pragma once
#include <System/DirectX.h>
#include <MyMath.h>

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(const UINT width, const UINT height, const DXGI_FORMAT texFormat, const DXGI_FORMAT rtvFormat, const DrawPass::Kind kind);
	ID3D11ShaderResourceView* GetSRV();
	inline ID3D11RenderTargetView* GetRTV() { return m_pRTV; }
private:
	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pSRV;
	int m_nNum;
};
