#include "Texture.h"
#include "DirectXTex/DirectXTex.h"
#include <iostream>
#include <System/DebugLog.h>

#ifdef _X86_
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Win32/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/Win32/Release/DirectXTex.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/x64/Release/DirectXTex.lib")
#endif
#endif

HRESULT LoadTextureFromFile(const char *pszFileName, ID3D11ShaderResourceView **ppTexture)
{
	HRESULT hr;
	ID3D11Device *pDevice = GetDevice();
	ID3D11DeviceContext *pContext = GetContext();
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;

	// 文字変換
	MultiByteToWideChar(0, 0, pszFileName, -1, wPath, MAX_PATH);
	// ファイル別読み込み
	if (strstr(pszFileName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	}
	if (FAILED(hr))
	{
		DebugLog::GetInstance().LoadFileError(pszFileName);
		//std::cout << pszFileName << "が読み込めませんでした" << std::endl;

		MessageBox(NULL, pszFileName, "Failed to load texture.", MB_OK);
		return hr;
	}

	// シェーダリソースビュー作成
	hr = CreateShaderResourceView(
		pDevice,
		image.GetImages(), image.GetImageCount(),
		mdata, ppTexture);

	return hr;
}

HRESULT CreateRenderTexture(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11ShaderResourceView** ppTexture, ID3D11RenderTargetView** ppRTV)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();

	//--- テクスチャ
	// 設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	// 生成
	ID3D11Texture2D* tex2D;
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &tex2D);
	if (FAILED(hr)) { return E_FAIL; }

	//--- レンダーターゲット
	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// 生成
	hr = pDevice->CreateRenderTargetView(tex2D, &rtvDesc, ppRTV);
	if (FAILED(hr)) { return E_FAIL; }

	//--- シェーダリソース
	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// 生成
	hr = pDevice->CreateShaderResourceView(tex2D, &srvDesc, ppTexture);
	if (FAILED(hr)) { return E_FAIL; }

	return hr;
}


HRESULT CreateDepthStencil(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11DepthStencilView** ppDSV)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();

	//--- テクスチャ
	// 設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	// 生成
	ID3D11Texture2D* tex2D;
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &tex2D);
	if (FAILED(hr)) { return E_FAIL; }

	//--- デプスステンシル
	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	// 生成
	hr = pDevice->CreateDepthStencilView(tex2D, &dsvDesc, ppDSV);
	if (FAILED(hr)) { return E_FAIL; }

	return hr;
}