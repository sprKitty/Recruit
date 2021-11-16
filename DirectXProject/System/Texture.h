#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "DirectX.h"

HRESULT LoadTextureFromFile(const char *pszFileName, ID3D11ShaderResourceView **ppTexture);
HRESULT CreateRenderTexture(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11ShaderResourceView** ppTexture, ID3D11RenderTargetView** ppRTV);
HRESULT CreateDepthStencil(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11DepthStencilView** ppDSV);
#endif // __TEXTURE_H__