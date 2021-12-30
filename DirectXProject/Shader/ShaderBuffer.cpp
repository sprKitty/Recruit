#include <Shader/ShaderBuffer.h>
#include <System/Shader.h>
#include <System/Texture.h>
#include <MyMath.h>
#include <Defines.h>


const char* pVSPath[] =
{
	"Assets/VSNormal.cso",
	"Assets/VSTriPlanar.cso",
	"Assets/VSLightDepth.cso",
	"Assets/VSCameraDepth.cso",
	"Assets/VSGaussianBlur.cso",
	"Assets/VSWaterReflection.cso",
};
static_assert(!(static_cast<int>(VS_TYPE::MAX) < _countof(pVSPath)), "VSKindへの定義追加忘れ");
static_assert(!(static_cast<int>(VS_TYPE::MAX) > _countof(pVSPath)), "VSPathへの読込ファイル追加忘れ");

const char* pPSPath[] =
{
	"Assets/PSNormal.cso",
	"Assets/PSEffect.cso",
	"Assets/PSTriPlanar.cso",
	"Assets/PSCharacter.cso",
	"Assets/PSPixelColor.cso",
	"Assets/PSLightDepth.cso",
	"Assets/PSCameraDepth.cso",
	"Assets/PSGaussianBlur.cso",
	"Assets/PSKawaseBloom.cso",
	"Assets/PSMixTexture.cso",
	"Assets/PSDOF.cso",
	"Assets/PSWaterReflection.cso",
};
static_assert(!(static_cast<int>(PS_TYPE::MAX) < _countof(pPSPath)), "PSKindへの定義追加忘れ");
static_assert(!(static_cast<int>(PS_TYPE::MAX) > _countof(pPSPath)), L"PSPathへの読込ファイル追加忘れ");

ShaderBuffer::ShaderBuffer()
{
	//for (int i = 0; i < 250; ++i)
	//{
	//	DirectX::XMStoreFloat4x4(&m_vs2.bone[i], DirectX::XMMatrixIdentity());
	//}
}

ShaderBuffer::~ShaderBuffer()
{
}

void ShaderBuffer::Initialize()
{
	HRESULT hr;
	// 頂点シェーダー作成
	m_pVSList.resize(static_cast<int>(VS_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(VS_TYPE::MAX); ++i)
	{
		m_pVSList[i].reset(new VertexShader());
		m_pVSList[i]->Create(pVSPath[i]);
	}

	// ピクセルシェーダー作成
	m_pPSList.resize(static_cast<int>(PS_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(PS_TYPE::MAX); ++i)
	{
		m_pPSList[i].reset(new PixelShader());
		m_pPSList[i]->Create(pPSPath[i]);
	}

	// コンスタンスバッファ作成
	m_pCBList.resize(static_cast<int>(CB_TYPE::MAX));
	for (int i = 0; i < static_cast<int>(CB_TYPE::MAX); ++i)
	{
		m_pCBList[i].reset(new ConstantBuffer());
	}

	UINT nSize = sizeof(DirectX::XMFLOAT4X4);
	hr = m_pCBList[CB_TYPE::INSTANCING_WORLD]->Create(nSize * INSTANCING_MAX);
	hr = m_pCBList[CB_TYPE::CAMEAR_VP]->Create(sizeof(ShaderResource::CameraVP));
	hr = m_pCBList[CB_TYPE::LIGHT_VP]->Create(sizeof(ShaderResource::LightVP));
	hr = m_pCBList[CB_TYPE::CAMERA_INFO]->Create(sizeof(ShaderResource::CameraInfo));
	hr = m_pCBList[CB_TYPE::LIGHT_INFO]->Create(sizeof(ShaderResource::LightInfo));
	hr = m_pCBList[CB_TYPE::TEX_SETTING]->Create(sizeof(ShaderResource::TexSetting));
	hr = m_pCBList[CB_TYPE::POSTEFFECT]->Create(sizeof(ShaderResource::PostEffect));


	m_pCBList[static_cast<int>(CB_TYPE::INSTANCING_WORLD)]->BindVS(0);
	m_pCBList[static_cast<int>(CB_TYPE::CAMEAR_VP)]->BindVS(1);
	m_pCBList[static_cast<int>(CB_TYPE::LIGHT_VP)]->BindVS(2);
	m_pCBList[static_cast<int>(CB_TYPE::LIGHT_INFO)]->BindVS(3);

	m_pCBList[static_cast<int>(CB_TYPE::CAMERA_INFO)]->BindPS(0);
	m_pCBList[static_cast<int>(CB_TYPE::LIGHT_INFO)]->BindPS(1);
	m_pCBList[static_cast<int>(CB_TYPE::TEX_SETTING)]->BindPS(2);
	m_pCBList[static_cast<int>(CB_TYPE::POSTEFFECT)]->BindPS(3);

	InitParam();

	Write(CB_TYPE::INSTANCING_WORLD);
}

void ShaderBuffer::Finalize()
{
	m_pVSList.clear();
	m_pPSList.clear();
	m_pCBList.clear();
}

void ShaderBuffer::InitParam()
{
	for (int i = 0; i < INSTANCING_MAX; ++i)
	{
		DirectX::XMStoreFloat4x4(&m_instancingWorld[i], DirectX::XMMatrixIdentity());
	}

	for (int i = 0; i < 2; ++i)
	{
		DirectX::XMStoreFloat4x4(&m_cameraVP.info[i].view, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_cameraVP.info[i].proj, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_lightVP.info[i].proj, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_lightVP.info[i].view, DirectX::XMMatrixIdentity());
	}

	//m_ps1.color = DirectX::XMFLOAT4(1, 1, 1, 1);
	//m_ps1.nBlur = 0;
	//m_ps1.nWidth = SCREEN_WIDTH;
	//m_ps1.nHeight = SCREEN_HEIGHT;
	//m_ps1.nDummy = 0;
	m_texSetting.tiling = DirectX::XMFLOAT2(1, 1);
	m_texSetting.offset = DirectX::XMFLOAT2(0, 0);
	m_texSetting.color = DirectX::XMFLOAT4(1, 1, 1, 1);
	m_texSetting.nWrap = 1;
	m_texSetting.fAlpha = 0;
	m_texSetting.fTime = 0;
	m_nSpotNext = 0;

	//m_postEffect.blur = { 0,0,0,0 };
	
	for (int i = 0; i < CB_TYPE::MAX; ++i)
	{
		Write(static_cast<CB_TYPE::Kind>(i));
	}
}

void ShaderBuffer::Write(const CB_TYPE::Kind cb)
{
	switch (cb)
	{
	case CB_TYPE::INSTANCING_WORLD:
		m_pCBList[cb]->Write(&m_instancingWorld);
		break;

	case CB_TYPE::CAMEAR_VP:
		m_pCBList[cb]->Write(&m_cameraVP);
		break;

	case CB_TYPE::LIGHT_VP:
		m_pCBList[cb]->Write(&m_lightVP);
		break;
	
	case CB_TYPE::CAMERA_INFO:
		m_pCBList[cb]->Write(&m_cameraInfo);
		break;

	case CB_TYPE::LIGHT_INFO:
		m_pCBList[cb]->Write(&m_lightInfo);
		break;

	case CB_TYPE::TEX_SETTING:
		m_pCBList[cb]->Write(&m_texSetting);
		break;

	case CB_TYPE::POSTEFFECT:
		m_pCBList[cb]->Write(&m_postEffect);
		break;

	case CB_TYPE::MAX:
	default:
		break;
	}
}

void ShaderBuffer::BindVS(const VS_TYPE::Kind vs)
{
	m_pVSList[vs]->Bind();
}

void ShaderBuffer::BindPS(const PS_TYPE::Kind ps)
{
	m_pPSList[ps]->Bind();
}

void ShaderBuffer::SetTexture(ID3D11ShaderResourceView * pTex, const ShaderResource::TEX_TYPE type)
{
	ID3D11DeviceContext* pContext = DirectX11::GetInstance().GetContext();
	pContext->PSSetShaderResources(static_cast<int>(type), 1, &pTex);
}

void ShaderBuffer::SetTexTilingOffset(const Vector2& scale, const Vector2& offset)
{
	Vector2 s = scale;
	Vector2 o = offset;
	m_texSetting.tiling = s.Convert();
	m_texSetting.offset = o.Convert();
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetMultiplyColor(const Vector4 & color)
{
	Vector4 c = color;
	m_texSetting.color = c.Convert();
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetGrayScaleAlpha(const float fAlpha)
{
	m_texSetting.fAlpha = fAlpha;
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetTime(const float time)
{
	m_texSetting.fTime = time;
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetGameStartCnt(const float time)
{
	m_texSetting.fGameStartCnt = time;
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetTexSamplerWRAP()
{
	m_texSetting.nWrap = 1;
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetTexSamplerCRAMP()
{
	m_texSetting.nWrap = 0;
	Write(CB_TYPE::TEX_SETTING);
}

void ShaderBuffer::SetWorld(const DirectX::XMMATRIX & world)
{
	DirectX::XMStoreFloat4x4(&m_instancingWorld[0], DirectX::XMMatrixTranspose(world));
	Write(CB_TYPE::INSTANCING_WORLD);
}

void ShaderBuffer::SetInstancingWorld(const Vector3& vScl, const Vector3& vRot, const std::vector<Vector3>& posList)
{
	for (int i = 0; i < posList.size(); ++i)
	{
		if (i >= INSTANCING_MAX)break;
		DirectX::XMStoreFloat4x4(&m_instancingWorld[i], DirectX::XMMatrixTranspose(MyMath::ConvertMatrix(vScl, vRot, posList[i])));
	}
	Write(CB_TYPE::INSTANCING_WORLD);
}

void ShaderBuffer::SetCameraVP(const DirectX::XMMATRIX & mView, const DirectX::XMMATRIX & mProj, const int num)
{
	if (num >= ShaderResource::VPBUFFER_SIZE)return;

	DirectX::XMStoreFloat4x4(&m_cameraVP.info[num].view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&m_cameraVP.info[num].proj, DirectX::XMMatrixTranspose(mProj));
	Write(CB_TYPE::CAMEAR_VP);
}

void ShaderBuffer::SetLightVP(const DirectX::XMMATRIX & mView, const DirectX::XMMATRIX & mProj, const int num)
{
	if (num >= ShaderResource::VPBUFFER_SIZE)return;

	DirectX::XMStoreFloat4x4(&m_lightVP.info[num].view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&m_lightVP.info[num].proj, DirectX::XMMatrixTranspose(mProj));
	Write(CB_TYPE::LIGHT_VP);
}

void ShaderBuffer::SetLightInfo(const ShaderResource::LightInfo & light)
{
	m_lightInfo = light;
	Write(CB_TYPE::CAMERA_INFO);
}

void ShaderBuffer::SetCameraInfo(const ShaderResource::CameraInfo & camera)
{
	m_cameraInfo = camera;
	Write(CB_TYPE::CAMERA_INFO);
}

void ShaderBuffer::SetPostEffectInfo(const ShaderResource::PostEffect & post)
{
	m_postEffect = post;
	Write(CB_TYPE::POSTEFFECT);
}
