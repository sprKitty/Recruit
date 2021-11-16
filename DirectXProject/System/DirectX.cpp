#include "DirectX.h"

//--- �O���[�o���ϐ�
ID3D11Device *g_pDevice;
ID3D11DeviceContext *g_pContext;
IDXGISwapChain *g_pSwapChain;

// ��ʂւ̏o��
ID3D11RenderTargetView* g_pRenderTarget;
ID3D11DepthStencilView*	g_pDepthStencil;

ID3D11BlendState* g_pBlendState[BLEND_MAX];
ID3D11RasterizerState* g_pRasterizer[CULL_MAX];

RenderTargetKind g_RTV;

HRESULT InitDX(HWND hWnd, UINT width, UINT height, bool fullscreen)
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
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,		// �����x(�ᑬ
		D3D_DRIVER_TYPE_REFERENCE,	// CPU�ŕ`��
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	// createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	// �@�\���x��
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
			&g_pSwapChain,			// IDXGIDwapChain�C���^�t�F�[�X	
			&g_pDevice,				// ID3D11Device�C���^�t�F�[�X
			&featureLevel,		// �T�|�[�g����Ă���@�\���x��
			&g_pContext);		// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}

	//--- �����_�����O�^�[�Q�b�g
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) { return hr; }
	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTarget);
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) { return hr; }

	//--- �[�x�o�b�t�@�̍쐬
	// �[�x�o�b�t�@�Ƃ��Ďg�p����
	// �e�N�X�`���̍쐬
	ID3D11Texture2D* pDepthTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.Width = width;
	depthTexDesc.Height = height;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.SampleDesc = sd.SampleDesc;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	// �e�N�X�`���̗��p���@
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	hr = g_pDevice->CreateTexture2D(&depthTexDesc, nullptr, &pDepthTexture);
	if (FAILED(hr)) { return E_FAIL; }

	// �쐬�����e�N�X�`����p����
	// �[�x�o�b�t�@�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthTexDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = g_pDevice->CreateDepthStencilView(pDepthTexture, &dsvDesc, &g_pDepthStencil);
	if (FAILED(hr)) { return E_FAIL; }

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
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		blendDesc.RenderTarget[0].SrcBlend = blendSetting[i][0];
		blendDesc.RenderTarget[0].DestBlend = blendSetting[i][1];
		blendDesc.RenderTarget[0].BlendOp = blendOp[i][0];
		blendDesc.RenderTarget[0].SrcBlendAlpha = blendSetting[i][2];
		blendDesc.RenderTarget[0].DestBlendAlpha = blendSetting[i][3];
		blendDesc.RenderTarget[0].BlendOpAlpha = blendOp[i][1];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetBlendMode(BLEND_ALPHA);

	//--- �T���v���[
	D3D11_SAMPLER_DESC smpDescWRAP;
	D3D11_SAMPLER_DESC smpDescMIRROR;
	D3D11_SAMPLER_DESC smpDescCLAMP;
	D3D11_SAMPLER_DESC smpDescBORDER;
	D3D11_SAMPLER_DESC smpDescSoftShadow;
	ID3D11SamplerState *pSamplerWRAP;
	ID3D11SamplerState *pSamplerMIRROR;
	ID3D11SamplerState *pSamplerCRAMP;
	ID3D11SamplerState *pSamplerBORDER;
	ID3D11SamplerState *pSamplerSofShadow;

	ZeroMemory(&smpDescWRAP, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescMIRROR, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescCLAMP, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescBORDER, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&smpDescSoftShadow, sizeof(D3D11_SAMPLER_DESC));

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
	
	smpDescSoftShadow = smpDescCLAMP;
	smpDescSoftShadow.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	smpDescSoftShadow.ComparisonFunc = D3D11_COMPARISON_GREATER;
	smpDescSoftShadow.MaxAnisotropy = 1;

	hr = g_pDevice->CreateSamplerState(&smpDescWRAP, &pSamplerWRAP);
	if (FAILED(hr))return hr;
	g_pContext->PSSetSamplers(0, 1, &pSamplerWRAP);
	SAFE_RELEASE(pSamplerWRAP);

	hr = g_pDevice->CreateSamplerState(&smpDescCLAMP, &pSamplerCRAMP);
	if (FAILED(hr))return hr;
	g_pContext->PSSetSamplers(1, 1, &pSamplerCRAMP);
	SAFE_RELEASE(pSamplerCRAMP);

	hr = g_pDevice->CreateSamplerState(&smpDescBORDER, &pSamplerBORDER);
	if (FAILED(hr))return hr;
	g_pContext->PSSetSamplers(2, 1, &pSamplerBORDER);
	SAFE_RELEASE(pSamplerBORDER);

	hr = g_pDevice->CreateSamplerState(&smpDescSoftShadow, &pSamplerSofShadow);
	if (FAILED(hr))return hr;
	g_pContext->PSSetSamplers(3, 1, &pSamplerSofShadow);
	SAFE_RELEASE(pSamplerSofShadow);

	hr = g_pDevice->CreateSamplerState(&smpDescMIRROR, &pSamplerMIRROR);
	if (FAILED(hr))return hr;
	g_pContext->PSSetSamplers(4, 1, &pSamplerMIRROR);
	SAFE_RELEASE(pSamplerMIRROR);

	//--- ���X�^���C�Y
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	hr = g_pDevice->CreateRasterizerState(&rasterDesc, &g_pRasterizer[CULL_NONE]);
	if (FAILED(hr)) { return hr; }
	rasterDesc.CullMode = D3D11_CULL_FRONT;

	hr = g_pDevice->CreateRasterizerState(&rasterDesc, &g_pRasterizer[CULL_FRONT]);
	if (FAILED(hr)) { return hr; }
	rasterDesc.CullMode = D3D11_CULL_BACK;

	hr = g_pDevice->CreateRasterizerState(&rasterDesc, &g_pRasterizer[CULL_BACK]);
	if (FAILED(hr)) { return hr; }
	SetCulling(CULL_BACK);


	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pContext->RSSetViewports(1, &vp);

	return S_OK;
}

void UninitDX()
{
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		SAFE_RELEASE(g_pBlendState[i]);
	}

	SAFE_RELEASE(g_pRasterizer[0]);
	SAFE_RELEASE(g_pRasterizer[1]);

	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pRenderTarget);

	g_pContext->ClearState();
	SAFE_RELEASE(g_pContext);

	g_pSwapChain->SetFullscreenState(false, nullptr);
	SAFE_RELEASE(g_pSwapChain);

	SAFE_RELEASE(g_pDevice);
}


/**
* @brief �`��N���A
* @param[in] color �`��F
*/
void BeginDrawDX()
{
	float color[4] = { 0.8f, 0.8f, 0.9f,1.0f };

	g_pContext->OMSetRenderTargets(1, &g_pRenderTarget, g_pDepthStencil);
	g_pContext->ClearRenderTargetView(g_pRenderTarget, color);
	g_pContext->ClearDepthStencilView(g_pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void EndDrawDX()
{
	g_pSwapChain->Present(0, 0);
}


ID3D11Device *GetDevice()
{
	return g_pDevice;
}


ID3D11DeviceContext *GetContext()
{
	return g_pContext;
}


ID3D11DepthStencilView * GetDepthStencil()
{
	return g_pDepthStencil;
}

void SetBlendMode(BlendMode blend)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pContext->OMSetBlendState(g_pBlendState[blend], blendFactor, 0xffffffff);
}

void SetCulling(CullingMode cull)
{
	g_pContext->RSSetState(g_pRasterizer[cull]);
}

void SetZBuffer(bool enable)
{
	g_pContext->OMSetRenderTargets(1, &g_pRenderTarget, enable ? g_pDepthStencil : nullptr);
}
