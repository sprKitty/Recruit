#include <Shader/ShaderBuffer.h>
#include <System/Shader.h>
#include <System/Texture.h>
#include <MyMath.h>
#include <Defines.h>


const char* pVSPath[] =
{
	"Assets/VSNormal.cso",
	"Assets/VSAnimation.cso",
	"Assets/VSLightDepth.cso",
	"Assets/VSCameraDepth.cso",
	"Assets/VSWater.cso",
	"Assets/VSOutLine.cso",
	"Assets/VSDummy.cso"
};
static_assert(!(static_cast<int>(VS_TYPE::MAX) < _countof(pVSPath)), "VSKindへの定義追加忘れ");
static_assert(!(static_cast<int>(VS_TYPE::MAX) > _countof(pVSPath)), "VSPathへの読込ファイル追加忘れ");

const char* pPSPath[] =
{
	"Assets/PSNormal.cso",
	"Assets/PSBackBuffer.cso",
	"Assets/PSPixelColor.cso",
	"Assets/PSScope.cso",
	"Assets/PSBlur.cso",
	"Assets/PSToon.cso",
	"Assets/PSToonShadow.cso",
	"Assets/PSWorld.cso",
	"Assets/PSLightDepth.cso",
	"Assets/PSCameraDepth.cso",
	"Assets/PSFade.cso",
	"Assets/PSWater.cso"
};
static_assert(!(static_cast<int>(PS_TYPE::MAX) < _countof(pPSPath)), "PSKindへの定義追加忘れ");
static_assert(!(static_cast<int>(PS_TYPE::MAX) > _countof(pPSPath)), L"PSPathへの読込ファイル追加忘れ");

ShaderBuffer::ShaderBuffer()
{
	for (int i = 0; i < 250; ++i)
	{
		DirectX::XMStoreFloat4x4(&m_vs2.bone[i], DirectX::XMMatrixIdentity());
	}
}

ShaderBuffer::~ShaderBuffer()
{
	if (m_pMap) { m_pMap->Release(); }
}

void ShaderBuffer::Initialize()
{
	HRESULT hr;
	// 頂点シェーダー作成
	m_pVSList.resize(static_cast<int>(VS_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(VS_TYPE::MAX); ++i)
	{
		m_pVSList[i].reset(new VertexShader());
	}

	m_pVSList[static_cast<int>(VS_TYPE::ANIMATION)]->SetLayoutType(VertexShader::ANIMATION);

	for (int i = 0; i < static_cast<int>(VS_TYPE::MAX); ++i)
	{
		FnAssert(m_pVSList[i]->Create(pVSPath[i]), "頂点シェーダ読み込み失敗");
		//g_vsList[i]->Create(pVSPath[i]);
	}

	// ピクセルシェーダー作成
	m_pPSList.resize(static_cast<int>(PS_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(PS_TYPE::MAX); ++i)
	{
		m_pPSList[i].reset(new PixelShader());
		FnAssert(m_pPSList[i]->Create(pPSPath[i]), "ピクセルシェーダ読み込み失敗");
		//g_psList[i]->Create(pPSPath[i]);
	}

	// コンスタンスバッファ作成
	m_pCBList.resize(static_cast<int>(CB_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(CB_TYPE::MAX); ++i)
	{
		m_pCBList[i].reset(new ConstantBuffer());
	}

	hr = m_pCBList[static_cast<int>(CB_TYPE::WORLD)    ]->Create(sizeof(DirectX::XMFLOAT4X4));
	hr = m_pCBList[static_cast<int>(CB_TYPE::V_REGIST1)]->Create(sizeof(ShaderResource::VSRegist1));
	hr = m_pCBList[static_cast<int>(CB_TYPE::V_REGIST2)]->Create(sizeof(ShaderResource::VSRegist2));
	hr = m_pCBList[static_cast<int>(CB_TYPE::P_REGIST0)]->Create(sizeof(ShaderResource::PSRegist0));
	hr = m_pCBList[static_cast<int>(CB_TYPE::P_REGIST1)]->Create(sizeof(ShaderResource::PSRegist1));
	hr = m_pCBList[static_cast<int>(CB_TYPE::P_REGIST2)]->Create(sizeof(ShaderResource::PSRegist2));


	m_pCBList[static_cast<int>(CB_TYPE::WORLD)]->BindVS(0);
	m_pCBList[static_cast<int>(CB_TYPE::V_REGIST1)]->BindVS(1);
	m_pCBList[static_cast<int>(CB_TYPE::V_REGIST2)]->BindVS(2);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST0)]->BindPS(0);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST1)]->BindPS(1);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST2)]->BindPS(2);

	InitParam();


	LoadTextureFromFile("Assets/ToonMap.png", &m_pMap);
	SetToonMap(m_pMap);
}

void ShaderBuffer::Finalize()
{
	m_pVSList.clear();
	m_pPSList.clear();
	m_pCBList.clear();
}

void ShaderBuffer::InitParam()
{
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vs1.camView, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vs1.camProj, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vs1.parallelLightView, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vs1.parallelLightView, DirectX::XMMatrixIdentity());

	m_ps1.color = DirectX::XMFLOAT4(1, 1, 1, 1);
	m_ps1.nBlur = 0;
	m_ps1.nWidth = SCREEN_WIDTH;
	m_ps1.nHeight = SCREEN_HEIGHT;
	m_ps1.nDummy = 0;
	m_ps2.tiling = DirectX::XMFLOAT2(1, 1);
	m_ps2.offset = DirectX::XMFLOAT2(0, 0);
	m_ps2.color = DirectX::XMFLOAT4(1, 1, 1, 1);
	m_ps2.nWrap = 1;
	m_ps2.fAlpha = 0;
	m_ps2.fTime = 0;
	m_pMap = nullptr;
	m_nSpotNext = 0;

	m_pCBList[static_cast<int>(CB_TYPE::WORLD)]->Write(&m_world);
	m_pCBList[static_cast<int>(CB_TYPE::V_REGIST1)]->Write(&m_vs1);
	m_pCBList[static_cast<int>(CB_TYPE::V_REGIST2)]->Write(&m_vs2);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST0)]->Write(&m_ps0);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST1)]->Write(&m_ps1);
	m_pCBList[static_cast<int>(CB_TYPE::P_REGIST2)]->Write(&m_ps2);
}

void ShaderBuffer::Write(CB_TYPE cb)
{
	int nArray = static_cast<int>(cb);
	switch (cb)
	{
	case CB_TYPE::WORLD:
		m_pCBList[nArray]->Write(&m_world);
		break;

	case CB_TYPE::V_REGIST1:
		m_pCBList[nArray]->Write(&m_vs1);
		break;

	case CB_TYPE::V_REGIST2:
		m_pCBList[nArray]->Write(&m_vs2);
		break;
	
	case CB_TYPE::P_REGIST0:
		m_pCBList[nArray]->Write(&m_ps0);
		break;

	case CB_TYPE::P_REGIST1:
		m_pCBList[nArray]->Write(&m_ps1);
		break;

	case CB_TYPE::P_REGIST2:
		m_pCBList[nArray]->Write(&m_ps2);
		break;

	case CB_TYPE::MAX:
	default:
		break;
	}
}

void ShaderBuffer::BindVS(VS_TYPE vs)
{
	m_pVSList[static_cast<int>(vs)]->Bind();
}

void ShaderBuffer::BindPS(PS_TYPE ps)
{
	m_pPSList[static_cast<int>(ps)]->Bind();
}

void ShaderBuffer::SetTexture(ID3D11ShaderResourceView * pTex, ShaderResource::TEX_TYPE type)
{
	ID3D11DeviceContext* pContext = GetContext();
	pContext->PSSetShaderResources(static_cast<int>(type), 1, &pTex);
}

void ShaderBuffer::SetTexTilingOffset(Vector2& scale, Vector2& offset)
{
	m_ps2.tiling = scale.Convert();
	m_ps2.offset =offset.Convert();
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetMultiplyColor(Vector4 & color)
{
	m_ps2.color = color.Convert();
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetGrayScaleAlpha(float fAlpha)
{
	m_ps2.fAlpha = fAlpha;
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetTime(float & time)
{
	m_ps2.fTime = time;
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetTexSamplerWRAP()
{
	m_ps2.nWrap = 1;
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetTexSamplerCRAMP()
{
	m_ps2.nWrap = 0;
	Write(CB_TYPE::P_REGIST2);
}

void ShaderBuffer::SetWorld(const DirectX::XMMATRIX & world)
{
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixTranspose(world));
	Write(CB_TYPE::WORLD);
}

void ShaderBuffer::SetCameraVP(const DirectX::XMMATRIX & mView, const DirectX::XMMATRIX & mProj)
{
	DirectX::XMStoreFloat4x4(&m_vs1.camView, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&m_vs1.camProj, DirectX::XMMatrixTranspose(mProj));
	Write(CB_TYPE::V_REGIST1);
}

void ShaderBuffer::SetParallelLightVP(const DirectX::XMMATRIX & mView, const DirectX::XMMATRIX & mProj)
{
	DirectX::XMStoreFloat4x4(&m_vs1.parallelLightView, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&m_vs1.parallelLightProj, DirectX::XMMatrixTranspose(mProj));
	Write(CB_TYPE::V_REGIST1);
}

void ShaderBuffer::SetAnimeBone(const DirectX::XMFLOAT4X4 * pBone)
{
	for (int i = 0; i < 250; ++i)
	{
		m_vs2.bone[i] = pBone[i];
	}
	Write(CB_TYPE::V_REGIST2);
}

void ShaderBuffer::SetColor(const DirectX::XMFLOAT4 & color)
{
	m_ps1.color = color;
	Write(CB_TYPE::P_REGIST1);
}

void ShaderBuffer::SetParallelLightInfo(const ShaderResource::LightInfo & light)
{
	m_ps0.parallelLI = light;
	Write(CB_TYPE::P_REGIST0);
}

void ShaderBuffer::SetCameraInfo(const ShaderResource::CameraInfo & camera)
{
	m_ps0.camInfo = camera;
	Write(CB_TYPE::P_REGIST0);
}

void ShaderBuffer::SetBlur(bool flg)
{
	m_ps1.nBlur = flg;
	Write(CB_TYPE::P_REGIST1);
}

void ShaderBuffer::SetToonMap(ID3D11ShaderResourceView* pMap)
{
	ID3D11DeviceContext* pContext = GetContext();
	pContext->PSSetShaderResources(static_cast<int>(ShaderResource::TEX_TYPE::TOONMAP), 1, &pMap);
}