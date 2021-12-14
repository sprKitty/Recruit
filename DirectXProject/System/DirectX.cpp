#include "DirectX.h"


void DirectX11::Finalize()
{
	for (int i = 0; i < BlendMode::BLEND_MAX; ++i)
	{
		SAFE_RELEASE(m_pBlendState[i]);
	}

	SAFE_RELEASE(m_pRasterizer[0]);
	SAFE_RELEASE(m_pRasterizer[1]);

	m_pDepthStencil.reset();
	SAFE_RELEASE(m_pBBRT);

	m_pContext->ClearState();
	SAFE_RELEASE(m_pContext);

	m_pSwapChain->SetFullscreenState(false, nullptr);
	SAFE_RELEASE(m_pSwapChain);

	SAFE_RELEASE(m_pDevice);
}

const HRESULT  DirectX11::Start(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));					// �[���N���A
	sd.BufferDesc.Width = width;						// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = height;						// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B,A)
	sd.SampleDesc.Count = 1;		// �}���`�T���v���̐�
	sd.BufferDesc.RefreshRate.Numerator = 1000;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
	sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	sd.OutputWindow = hWnd;			// �֘A�t����E�C���h�E
	sd.Windowed = fullscreen ? FALSE : TRUE;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,		// �����x(�ᑬ
		D3D_DRIVER_TYPE_REFERENCE,	// CPU�ŕ`��
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,					// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�i�m�t�k�k�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			driverType,				// �f�o�C�X�h���C�o�̃^�C�v
			nullptr,					// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,		// �f�o�C�X�t���O
			featureLevels,			// �@�\���x��
			numFeatureLevels,		// �@�\���x����
			D3D11_SDK_VERSION,		// 
			&sd,					// �X���b�v�`�F�C���̐ݒ�
			&m_pSwapChain,			// IDXGIDwapChain�C���^�t�F�[�X	
			&m_pDevice,				// ID3D11Device�C���^�t�F�[�X
			&featureLevel,		// �T�|�[�g����Ă���@�\���x��
			&m_pContext);		// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr)) {
			break;
		}
	}

	//--- �����_�����O�^�[�Q�b�g
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) { return hr; }
	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pBBRT);
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) { return hr; }

	m_pDepthStencil.reset(new MyDepthStencil());
	m_pDepthStencil->Create(width, height, DrawPass::BACKBUFFER);

	//--- �A���t�@�u�����h
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	D3D11_BLEND blendSetting[][4] =
	{
		{
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_INV_SRC_ALPHA,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ZERO
		},
		{
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ZERO
		},
	};
	D3D11_BLEND_OP blendOp[][2] =
	{
		{ D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_ADD },
		{ D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_ADD },
	};
	for (int i = 0; i < BlendMode::BLEND_MAX; ++i)
	{
		blendDesc.RenderTarget[0].SrcBlend = blendSetting[i][0];
		blendDesc.RenderTarget[0].DestBlend = blendSetting[i][1];
		blendDesc.RenderTarget[0].BlendOp = blendOp[i][0];
		blendDesc.RenderTarget[0].SrcBlendAlpha = blendSetting[i][2];
		blendDesc.RenderTarget[0].DestBlendAlpha = blendSetting[i][3];
		blendDesc.RenderTarget[0].BlendOpAlpha = blendOp[i][1];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetBlendMode(BlendMode::BLEND_ALPHA);

	//--- �T���v���[
	D3D11_SAMPLER_DESC smpDescWRAP;
	D3D11_SAMPLER_DESC smpDescMIRROR;
	D3D11_SAMPLER_DESC smpDescCLAMP;
	D3D11_SAMPLER_DESC smpDescBORDER;
	ID3D11SamplerState *pSamplerWRAP;
	ID3D11SamplerState *pSamplerMIRROR;
	ID3D11SamplerState *pSamplerCRAMP;
	ID3D11SamplerState *pSamplerBORDER;

	ZeroMemory(&smpDescWRAP, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescMIRROR, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescCLAMP, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescBORDER, sizeof(D3D11_SAMPLER_DESC));

	// �g��k�����̐F�̎擾���@
	// D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDescWRAP.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDescMIRROR.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDescCLAMP.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// �e�N�X�`���̌J��Ԃ����@
	//WRAP
	smpDescWRAP.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDescWRAP.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDescWRAP.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// MIRROR
	smpDescMIRROR.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpDescMIRROR.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpDescMIRROR.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	// CLAMP
	smpDescCLAMP.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDescCLAMP.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDescCLAMP.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	smpDescBORDER.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDescBORDER.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDescBORDER.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	smpDescBORDER.BorderColor[0] = 1;
	smpDescBORDER.BorderColor[1] = 1;
	smpDescBORDER.BorderColor[2] = 1;
	smpDescBORDER.BorderColor[3] = 1;

	hr = m_pDevice->CreateSamplerState(&smpDescWRAP, &pSamplerWRAP);
	if (FAILED(hr))return hr;
	m_pContext->PSSetSamplers(0, 1, &pSamplerWRAP);
	SAFE_RELEASE(pSamplerWRAP);

	hr = m_pDevice->CreateSamplerState(&smpDescCLAMP, &pSamplerCRAMP);
	if (FAILED(hr))return hr;
	m_pContext->PSSetSamplers(1, 1, &pSamplerCRAMP);
	SAFE_RELEASE(pSamplerCRAMP);

	hr = m_pDevice->CreateSamplerState(&smpDescBORDER, &pSamplerBORDER);
	if (FAILED(hr))return hr;
	m_pContext->PSSetSamplers(2, 1, &pSamplerBORDER);
	SAFE_RELEASE(pSamplerBORDER);

	hr = m_pDevice->CreateSamplerState(&smpDescMIRROR, &pSamplerMIRROR);
	if (FAILED(hr))return hr;
	m_pContext->PSSetSamplers(4, 1, &pSamplerMIRROR);
	SAFE_RELEASE(pSamplerMIRROR);

	//--- ���X�^���C�Y
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterizer[CullingMode::CULL_NONE]);
	if (FAILED(hr)) { return hr; }
	rasterDesc.CullMode = D3D11_CULL_FRONT;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterizer[CullingMode::CULL_FRONT]);
	if (FAILED(hr)) { return hr; }
	rasterDesc.CullMode = D3D11_CULL_BACK;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterizer[CullingMode::CULL_BACK]);
	if (FAILED(hr)) { return hr; }
	SetCulling(CullingMode::CULL_BACK);

	// �r���[�|�[�g��ݒ�
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_vp.Width = (FLOAT)width;
	m_vp.Height = (FLOAT)height;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &m_vp);

	return S_OK;
}

void DirectX11::BeginDraw(D3D11_VIEWPORT* vp, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, Vector4* pColor)
{
	float color[4] = { 0.8f, 0.8f, 0.9f,1.0f };
	if (pColor)
	{
		color[0] = pColor->x;
		color[1] = pColor->y;
		color[2] = pColor->z;
		color[3] = pColor->w;
	}

	if (vp)
	{
		m_pContext->RSSetViewports(1, vp);
	}
	else
	{
		m_pContext->RSSetViewports(1, &m_vp);
	}

	if (pRTV)
	{
		m_pContext->ClearRenderTargetView(pRTV, color);
		if (pDSV)
		{
			m_pContext->OMSetRenderTargets(1, &pRTV, pDSV);
		}
		else
		{
			m_pContext->OMSetRenderTargets(1, &pRTV, m_pDepthStencil->Get());
		}
	}
	else
	{
		m_pContext->ClearRenderTargetView(m_pBBRT, color);
		if (pDSV)
		{
			m_pContext->OMSetRenderTargets(1, &m_pBBRT, pDSV);
		}
		else
		{
			m_pContext->OMSetRenderTargets(1, &m_pBBRT, m_pDepthStencil->Get());
		}
	}
	if (pDSV)
	{
		m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		m_pContext->ClearDepthStencilView(m_pDepthStencil->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void DirectX11::EndDraw()
{
	m_pSwapChain->Present(0, 0);
}

void DirectX11::SetBlendMode(const BlendMode::Kind kind)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pContext->OMSetBlendState(m_pBlendState[kind], blendFactor, 0xffffffff);
}

void DirectX11::SetCulling(CullingMode::Kind kind)
{
	m_pContext->RSSetState(m_pRasterizer[kind]);
}

const HRESULT MyDepthStencil::Create(const UINT width, const UINT height, const DrawPass::Kind kind)
{
	HRESULT hr;
	ID3D11Texture2D* pTexDepth = nullptr;
	D3D11_TEXTURE2D_DESC texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	hr = DirectX11::GetInstance().GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTexDepth);
	if (FAILED(hr)) { return E_FAIL; }

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDescShadow = {};
	ZeroMemory(&dsvDescShadow, sizeof(dsvDescShadow));
	dsvDescShadow.Format = texDesc.Format;

	switch (kind)
	{
	case DrawPass::BACKBUFFER:
		dsvDescShadow.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		break;
	case DrawPass::MULTIPATH:
		dsvDescShadow.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		break;
	case DrawPass::MAX:
	default:
		break;
	}
	dsvDescShadow.Texture2D.MipSlice = 0;
	hr = DirectX11::GetInstance().GetDevice()->CreateDepthStencilView(pTexDepth, &dsvDescShadow, &m_pDepthStencil);
	if (FAILED(hr)) { return E_FAIL; }

	return S_OK;
}