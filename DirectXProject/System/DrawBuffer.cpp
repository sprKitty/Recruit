#include "DrawBuffer.h"
#include <Shader/ShaderBuffer.h>


DrawBuffer::DrawBuffer()
	: m_pVertexBuffer(NULL)
	, m_vtxSize(0)
	, m_vtxCount(0)
	, m_pIndexBuffer(NULL)
	, m_idxSize(0)
	, m_idxCount(0)
{
}
DrawBuffer::~DrawBuffer()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

HRESULT DrawBuffer::CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite)
{
	//--- 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	//--- バッファの初期値を設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pVtx;

	//--- 頂点バッファの作成
	HRESULT hr;
	ID3D11Device* pDevice = DirectX11::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pVertexBuffer);

	//--- そのほかの情報を設定
	if (SUCCEEDED(hr))
	{
		m_vtxSize = size;
		m_vtxCount = count;
	}
	return hr;
}

HRESULT DrawBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// インデックスバッファ生成
	ID3D11Device* pDevice = DirectX11::GetInstance().GetDevice();
	HRESULT hr;
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pIndexBuffer);
	if (SUCCEEDED(hr))
	{
		m_idxSize = size;
		m_idxCount = count;
	}

	return hr;
}

HRESULT DrawBuffer::CreateIndex(const long * pIdx, UINT count)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = sizeof(long) * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// インデックスバッファ生成
	ID3D11Device* pDevice = DirectX11::GetInstance().GetDevice();
	HRESULT hr;
	hr = pDevice->CreateBuffer(
		&bufDesc, &subResource,
		&m_pIndexBuffer);
	if (SUCCEEDED(hr))
	{
		m_idxCount = count;
	}

	return hr;
}

void DrawBuffer::Draw(D3D11_PRIMITIVE_TOPOLOGY primitive, const int nDrawNum)
{
	ID3D11DeviceContext* pContext = DirectX11::GetInstance().GetContext();
	UINT stride = m_vtxSize;
	UINT offset = 0;

	pContext->IASetPrimitiveTopology(primitive);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// 描画
	if (m_idxCount > 0)
	{
		DXGI_FORMAT format;
		switch (m_idxSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		default: return;
		}
		pContext->IASetIndexBuffer(m_pIndexBuffer, format, 0);
		pContext->DrawIndexedInstanced(m_idxCount, static_cast<UINT>(nDrawNum), 0, 0, 0);
	}
	else
	{
		// 頂点バッファのみで描画
		//ShaderBuffer::GetInstance().SetInstancingWorld(m_vtxCount);
		//pContext->Draw(m_vtxCount, 0);
		pContext->DrawInstanced(m_vtxCount, static_cast<UINT>(nDrawNum), 0, 0);
	}

}

void DrawBuffer::Draw1(D3D11_PRIMITIVE_TOPOLOGY primitive, int start, int count)
{
	ID3D11DeviceContext* pContext = DirectX11::GetInstance().GetContext();
	UINT stride = m_vtxSize;
	UINT offset = 0;

	pContext->IASetPrimitiveTopology(primitive);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// 描画
	if (m_idxCount > 0)
	{
		pContext->IASetIndexBuffer(m_pIndexBuffer,
			m_idxSize == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
			0);
		pContext->DrawIndexed(count ? count : m_idxCount, start, 0);
	}
	else
	{
		pContext->Draw(count ? count : m_vtxCount, start);
	}
}


HRESULT DrawBuffer::Write(void* pVtx)
{
	HRESULT hr;
	ID3D11Device* pDevice = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetDevice();
	ID3D11DeviceContext* pContext = DirectX11::GetInstance().GetContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;

	// データコピー
	hr = pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		rsize_t size = m_vtxCount * m_vtxSize;
		memcpy_s(mapResource.pData, size, pVtx, size);
		pContext->Unmap(m_pVertexBuffer, 0);
	}
	return hr;
}