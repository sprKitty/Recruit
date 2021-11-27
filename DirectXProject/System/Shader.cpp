#include "Shader.h"
#include <stdio.h>

//=====================
// ��{�V�F�[�_�[
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
	// �t�@�C���̒��g��ǂݍ���
	FILE*fp;
	fp = fopen(FileName, "rb");
	if (!fp) { return hr; }
	
	// �t�@�C���̃T�C�Y�𒲂ׂ�
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// �t�@�C���̒��g���������ɓǂݍ���
	char* pData = nullptr;

	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// �V�F�[�_�[�쐬
	hr = MakeShader(pData, fileSize);
	if (pData) delete[] pData;
	return hr;
}

void ShaderBase::Bind(void)
{
}

//=================
// ���_�V�F�[�_�[
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

	// �V�F�[�_�[�쐬
	hr = pDevice->CreateVertexShader(pData, size, nullptr,&m_pVS);

	if (FAILED(hr))
	{
		return hr;
	}

	/* �C���v�b�g���C�A�E�g
	CPU����GPU�ɒ��_�̏��𑗂�ہAGPU���ł͂ǂ����������_���������Ă���̂����ʂł��Ȃ�
	�����ŁA�ǂ����������_�\���̂ɂȂ��Ă��邩���`�������"�C���v�b�g���C�A�E�g"�ƌĂ΂����̂ɂȂ�B
	*/

	//VerTexShader.hlsl���󂯎�钸�_�\����
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
		// �C���v�b�g���C�A�E�g�쐬
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
// �s�N�Z���V�F�[�_�[
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

	// �V�F�[�_�[�쐬
	hr = pDevice->CreatePixelShader(pData, size, nullptr, &m_pPS);
	return hr;
}

void PixelShader::Bind(void)
{
	ID3D11DeviceContext *pContext = DirectX11::GetInstance().GetContext();
	pContext->PSSetShader(m_pPS, nullptr, 0);
}


//=====================
// �W�I���g���V�F�[�_�[
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
// �萔�o�b�t�@
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
	/* �萔�o�b�t�@�쐬���̒���
	�萔�o�b�t�@�̑傫����4�o�C�g��؂�(4�o�C�g�A���C�����g)
	�łȂ��ƍ쐬�ł��Ȃ�
	*/
	HRESULT hr;
	
	// �쐬����o�b�t�@�̏��
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	
	//--- ���_�o�b�t�@�̍쐬
	ID3D11Device* pDevice = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffer);
	
	return hr;
}

void ConstantBuffer::Write(void * pData)
{
	// �萔�o�b�t�@�ւ̏�������
	ID3D11DeviceContext* pContext = DirectX11::DirectX11::GetInstance().GetInstance().DirectX11::GetInstance().GetContext();
	pContext->UpdateSubresource(m_pBuffer, 0, nullptr, pData, 0, 0);
}

void ConstantBuffer::BindVS(UINT slot)
{
	/*
	�萔�o�b�t�@�̃f�[�^�𑗂��
	�ǂ̈ʒu�Ɋi�[���邩��ڂ̈���(StartSlot)�Ɏw�肷��
	hlsl�̃R�[�h�ł�register(bX)�Ńo�b�t�@�̊i�[�ʒu�����߂Ă���
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


