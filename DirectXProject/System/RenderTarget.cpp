#include "RenderTarget.h"
#include <System/DirectX.h>

RenderTarget::RenderTarget()
	: m_ppTex(nullptr)
	, m_ppRTV(nullptr)
{
}


RenderTarget::~RenderTarget()
{
	for (int i = 0; i < m_nNum; ++i)
	{
		SAFE_RELEASE(m_ppTex[i]);
		SAFE_RELEASE(m_ppRTV[i]);
	}
	if (m_ppTex)
		delete[] m_ppTex;
	if (m_ppRTV)
		delete[] m_ppRTV;
}


void RenderTarget::Create(int nNum, int width, int height, DXGI_FORMAT texFormat, DXGI_FORMAT rtvFormat)
{
	m_nNum = nNum;
	m_ppRTV = new ID3D11RenderTargetView*[m_nNum];
	m_ppTex = new ID3D11ShaderResourceView*[m_nNum];
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D** ppTex = new ID3D11Texture2D*[m_nNum];
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
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	for (int i = 0; i < m_nNum; ++i)
	{
		hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &ppTex[i]);
		hr = DirectX11::GetInstance().GetDevice()->CreateRenderTargetView(ppTex[i], &rtvDesc, &m_ppRTV[i]);
		hr = DirectX11::GetInstance().GetDevice()->CreateShaderResourceView(ppTex[i], &srvDesc, &m_ppTex[i]);
		SAFE_RELEASE(ppTex[i]);
	}
	delete[] ppTex;
}


void RenderTarget::Draw(Vector4 rgba, ID3D11DepthStencilView * pDepthStencil)
{
	float color[4] = { rgba.x,rgba.y ,rgba.z ,rgba.w };
	DirectX11::GetInstance().GetContext()->OMSetRenderTargets(m_nNum, m_ppRTV, pDepthStencil);
	for (int i = 0; i < m_nNum; ++i)
	{
		DirectX11::GetInstance().GetContext()->ClearRenderTargetView(m_ppRTV[i], color);
	}
	DirectX11::GetInstance().GetContext()->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

