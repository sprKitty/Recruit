#pragma once
#include <System/DirectX.h>
#include <MyMath.h>

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(int nNum,int width, int height, DXGI_FORMAT texFormat, DXGI_FORMAT rtvFormat);
	void Draw(Vector4 rgb, ID3D11DepthStencilView* pDepthStencil);
	ID3D11RenderTargetView* GetRTV(int num) { return m_ppRTV[num]; }
	ID3D11ShaderResourceView* GetTexture(int num) { return m_ppTex[num]; }
private:
	ID3D11RenderTargetView** m_ppRTV;
	ID3D11ShaderResourceView** m_ppTex;
	int m_nNum;
};
