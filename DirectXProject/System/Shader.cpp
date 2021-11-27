#include "Shader.h"
#include <stdio.h>

//=====================
// 基本シェーダー
//=====================
ShaderBase::ShaderBase()
{
}

ShaderBase::~ShaderBase()
{
}

HRESULT ShaderBase::Create(const char * FileName)
{
	HRESULT hr = E_FAIL;
	// ファイルの中身を読み込む
	FILE*fp;
	fp = fopen(FileName, "rb");
	if (!fp) { return hr; }
	
	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ファイルの中身をメモリに読み込み
	char* pData = nullptr;

	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// シェーダー作成
	hr = MakeShader(pData, fileSize);
	if (pData) delete[] pData;
	return hr;
}

void ShaderBase::Bind(void)
{
}

//=================
// 頂点シェーダー
//=================
VertexShader::VertexShader()
	:m_pVS(nullptr)
	, m_pInputLayOut(nullptr)
	, m_type(NORMAL)
{

}

VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pVS);
}

HRESULT VertexShader::MakeShader(void * pData, UINT size, int num)
{
	HRESULT hr;
	ID3D11Device *pDevice = DirectX11::GetInstance().GetDevice();

	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, nullptr,&m_pVS);

	if (FAILED(hr))
	{
		return hr;
	}

	/* インプットレイアウト
	CPUからGPUに頂点の情報を送る際、GPU側ではどういった頂点情報を持っているのか判別できない
	そこで、どういった頂点構造体になっているかを定義した情報が"インプットレイアウト"と呼ばれるものになる。
	*/

	//VerTexShader.hlslが受け取る頂点構造体
	D3D11_INPUT_ELEMENT_DESC layout[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,   0,                           0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,      0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TANGENT", 0,DXGI_FORMAT_R32G32B32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"BINORMAL", 0,DXGI_FORMAT_R32G32B32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	D3D11_INPUT_ELEMENT_DESC layoutAnimation[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,                           0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"INDEX",    0, DXGI_FORMAT_R32G32B32A32_SINT , 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	if (m_type == NORMAL)
	{
		// インプットレイアウト作成
		hr = pDevice->CreateInputLayout(layout, _countof(layout), pData, size, &m_pInputLayOut);
	}
	else
	{
		hr = pDevice->CreateInputLayout(layoutAnimation, _countof(layoutAnimation), pData, size, &m_pInputLayOut);
	}
	return hr;
}

void VertexShader::Bind(void)
{
	ID3D11DeviceContext *pContext = DirectX11::GetInstance().GetContext();
	pContext->VSSetShader(m_pVS, nullptr, 0);
	pContext->IASetInputLayout(m_pInputLayOut);
}


//=====================
// ピクセルシェーダー
//=====================
PixelShader::PixelShader()
	:m_pPS(nullptr)
{

}

PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}

HRESULT PixelShader::MakeShader(void * pData, UINT size, int num)
{
	HRESULT hr;
	ID3D11Device *pDevice = DirectX11::GetInstance().GetDevice();

	// シェーダー作成
	hr = pDevice->CreatePixelShader(pData, size, nullptr, &m_pPS);
	return hr;
}

void PixelShader::Bind(void)
{
	ID3D11DeviceContext *pContext = DirectX11::GetInstance().GetContext();
	pContext->PSSetShader(m_pPS, nullptr, 0);
}


//=====================
// ジオメトリシェーダー
//=====================
GeometoryShader::GeometoryShader()
	:m_pGS(nullptr)
{
}

GeometoryShader::~GeometoryShader()
{
	SAFE_RELEASE(m_pGS);
}

HRESULT GeometoryShader::MakeShader(void * pData, UINT size, int num)
{
	HRESULT hr;
	ID3D11Device *pDevice = DirectX11::GetInstance().GetDevice();

	hr = pDevice->CreateGeometryShader(pData, size, nullptr, &m_pGS);
	return hr;
}

void GeometoryShader::Bind(void)
{
	ID3D11DeviceContext *pContext = DirectX11::GetInstance().GetContext();
	pContext->GSSetShader(m_pGS, nullptr, 0);
}


//=====================
// 定数バッファ
//=====================
ConstantBuffer::ConstantBuffer()
	:m_pBuffer(nullptr)
{

}

ConstantBuffer::~ConstantBuffer()
{
	SAFE_RELEASE(m_pBuffer);
}

HRESULT ConstantBuffer::Create(UINT size)
{
	/* 定数バッファ作成時の注意
	定数バッファの大きさは4バイト区切り(4バイトアライメント)
	でないと作成できない
	*/
	HRESULT hr;
	
	// 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	
	//--- 頂点バッファの作成
	ID3D11Device* pDevice = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffer);
	
	return hr;
}

void ConstantBuffer::Write(void * pData)
{
	// 定数バッファへの書き込み
	ID3D11DeviceContext* pContext = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetContext();
	pContext->UpdateSubresource(m_pBuffer, 0, nullptr, pData, 0, 0);
}

void ConstantBuffer::BindVS(UINT slot)
{
	/*
	定数バッファのデータを送る際
	どの位置に格納するか一つ目の引数(StartSlot)に指定する
	hlslのコードではregister(bX)でバッファの格納位置を決めておく
	*/
	ID3D11DeviceContext* pContext = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetContext();
	pContext->VSSetConstantBuffers(slot, 1, &m_pBuffer);
}

void ConstantBuffer::BindPS(UINT slot)
{
	ID3D11DeviceContext* pContext = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetContext();
	pContext->PSSetConstantBuffers(slot, 1, &m_pBuffer);
}

void ConstantBuffer::BindGS(UINT slot)
{
	ID3D11DeviceContext* pContext = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetContext();
	pContext->GSSetConstantBuffers(slot, 1, &m_pBuffer);
}


