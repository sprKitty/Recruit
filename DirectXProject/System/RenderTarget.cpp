#include "RenderTarget.h"
#include <System/DirectX.h>

RenderTarget::RenderTarget()
	: m_pTex(nullptr)
	, m_pRTV(nullptr)
{
}


RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pRTV);
}

void RenderTarget::Create(int width, int height, DXGI_FORMAT texFormat, DXGI_FORMAT rtvFormat)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* pTex;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = texFormat;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = rtvFormat;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	srvDesc.Texture2D.MipLevels = 1;
	hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTex);
	hr = DirectX11::GetInstance().GetDevice()->CreateRenderTargetView(pTex, &rtvDesc, &m_pRTV);
	hr = DirectX11::GetInstance().GetDevice()->CreateShaderResourceView(pTex, &srvDesc, &m_pTex);
	SAFE_RELEASE(pTex);
}