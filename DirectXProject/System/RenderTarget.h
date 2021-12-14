#pragma once
#include <System/DirectX.h>
#include <MyMath.h>

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(int width, int height, DXGI_FORMAT texFormat, DXGI_FORMAT rtvFormat);
	ID3D11RenderTargetView* Get() { return m_pRTV; }
	ID3D11ShaderResourceView* GetTexture() { return m_pTex; }

private:
	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pTex;
	int m_nNum;
};
