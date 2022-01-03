#include "RenderTarget.h"
#include <System/DirectX.h>

RenderTarget::RenderTarget()
	:m_ppRTV(nullptr)
	, m_ppSRV(nullptr)
	, m_pDepthStencil(nullptr)
	, m_nNum(0)
{
}


RenderTarget::~RenderTarget()
{
	for (int i = 0; i < m_nNum; ++i)
	{
		SAFE_RELEASE(m_ppRTV[i]);
		SAFE_RELEASE(m_ppSRV[i]);
	}
	delete[] m_ppRTV;
	delete[] m_ppSRV;
	m_ppRTV = nullptr;
	m_ppSRV = nullptr;
	SAFE_RELEASE(m_pDepthStencil);
}

void RenderTarget::Create(const FLOAT width, const FLOAT height, const DXGI_FORMAT format, const Vector4& vClearColor)
{
	for (int i = 0; i < m_nNum; ++i)
	{
		SAFE_RELEASE(m_ppRTV[i]);
		SAFE_RELEASE(m_ppSRV[i]);
	}
	m_vClearColorList.clear();
	if (m_ppRTV)
	{
		delete[] m_ppRTV;
	}
	if (m_ppSRV)
	{
		delete[] m_ppSRV;
	}
	m_nNum = 1;
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* pTex;
	ID3D11RenderTargetView* pRTV;
	ID3D11ShaderResourceView* pSRV;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_vp.Width = width;
	m_vp.Height = height;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	m_vClearColorList.emplace_back(vClearColor);

	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = static_cast<UINT>(m_vp.Width);
	texDesc.Height = static_cast<UINT>(m_vp.Height);
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

	hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTex);
	hr = DirectX11::GetInstance().GetDevice()->CreateRenderTargetView(pTex, &rtvDesc, &pRTV);
	hr = DirectX11::GetInstance().GetDevice()->CreateShaderResourceView(pTex, &srvDesc, &pSRV);

	m_ppRTV = new ID3D11RenderTargetView*[m_nNum];
	m_ppSRV = new ID3D11ShaderResourceView*[m_nNum];
	for (int i = 0; i < m_nNum; ++i)
	{
		m_ppRTV[i] = std::move(pRTV);
		m_ppSRV[i] = std::move(pSRV);
	}

	SAFE_RELEASE(pTex);
}

void RenderTarget::Addition(const DXGI_FORMAT format, const Vector4& vClearColor)
{
	ID3D11RenderTargetView** ppRTV = std::move(m_ppRTV);
	ID3D11ShaderResourceView** ppSRV = std::move(m_ppSRV);
	m_vClearColorList.emplace_back(vClearColor);
	++m_nNum;
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* pTex;
	ID3D11RenderTargetView* pRTV;
	ID3D11ShaderResourceView* pSRV;

	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = static_cast<UINT>(m_vp.Width);
	texDesc.Height = static_cast<UINT>(m_vp.Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

	hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTex);
	hr = DirectX11::GetInstance().GetDevice()->CreateRenderTargetView(pTex, &rtvDesc, &pRTV);
	hr = DirectX11::GetInstance().GetDevice()->CreateShaderResourceView(pTex, &srvDesc, &pSRV);

	m_ppRTV = new ID3D11RenderTargetView*[m_nNum];
	m_ppSRV = new ID3D11ShaderResourceView*[m_nNum];
	for (int i = 0; i < m_nNum - 1; ++i)
	{
		m_ppRTV[i] = std::move(ppRTV[i]);
		m_ppSRV[i] = std::move(ppSRV[i]);
	}
	m_ppRTV[m_nNum - 1] = pRTV;
	m_ppSRV[m_nNum - 1] = pSRV;
	delete[] ppRTV;
	delete[] ppSRV;
	SAFE_RELEASE(pTex);
}

void RenderTarget::CreateDepthStencil(const DXGI_FORMAT depthFormat)
{
	HRESULT hr;
	ID3D11Texture2D* pTexDepth = nullptr;
	D3D11_TEXTURE2D_DESC texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = static_cast<UINT>(m_vp.Width);
	texDesc.Height = static_cast<UINT>(m_vp.Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = depthFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTexDepth);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDescShadow = {};
	ZeroMemory(&dsvDescShadow, sizeof(dsvDescShadow));
	dsvDescShadow.Format = texDesc.Format;
	dsvDescShadow.Texture2D.MipSlice = 0;
	dsvDescShadow.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = DirectX11::GetInstance().GetDevice()->CreateDepthStencilView(pTexDepth, &dsvDescShadow, &m_pDepthStencil);
}
