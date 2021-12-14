#pragma once
#include <DirectXMath.h>
#include <System/DirectX.h>
#include <System/ClassDesign/Singleton.h>
#include <Vector.h>
#include <vector>
#include <memory>

class VertexShader;
class PixelShader;
class ConstantBuffer;


namespace VS_TYPE
{
	enum Kind
	{
		NORMAL,
		TRIPLANAR,
		LIGHTDEPTH,

		MAX
	};
}

namespace PS_TYPE
{
	enum Kind
	{
		NORMAL,
		EFFECT,
		TRIPLANAR,
		CHARACTER,
		COLOR,
		LIGHTDEPTH,

		MAX
	};
}

namespace CB_TYPE
{
	enum Kind
	{
		WORLD = 0,
		CAMEAR_VP,
		LIGHT_VP,
		INSTANCING,
		CAMERA_INFO,
		LIGHT_INFO,
		TEX_SETTING,

		MAX
	};
}

namespace ShaderResource
{
	// テクスチャのサンプラー設定
	enum class SAMLER_TYPE
	{
		WRAP = 0,
		CRAMP = 1,
		BORDER = 2,
		MIRROR = 3,
	};

	enum class TEX_TYPE
	{
		MAIN = 0,
		SCREEN,
		DEPTH_OF_SAHDOW,
		DEPTH_OF_FIELD,
		BUMP,
		CLIP,
		BLUR,
		GRAYSCALE,
	};

	struct ViewProjection
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};

	struct CameraVP
	{
		ViewProjection info;
	};

	struct LightVP
	{
		ViewProjection info;
	};

	struct Instancing
	{
		static const int nSize = 100;
		DirectX::XMFLOAT4X4 world[nSize];
	};

	struct CameraInfo
	{
		DirectX::XMFLOAT4 pos;
	};

	struct LightInfo
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 decay;
	};

	struct Bone
	{
		DirectX::XMFLOAT4X4 bone[250];
	};

	struct MappingData
	{
		DirectX::XMFLOAT4 color;
		int nBlur;
		int nWidth;
		int nHeight;
		int nDummy;
	};

	// テクスチャの設定
	struct TexSetting
	{
		DirectX::XMFLOAT2 tiling;
		DirectX::XMFLOAT2 offset;
		DirectX::XMFLOAT4 color;
		int nWrap;
		float fAlpha;
		float fTime;
		float fDummy;
	};
};

class ShaderBuffer : public Singleton<ShaderBuffer>
{
public:
	friend class Singleton<ShaderBuffer>;

public:

	void Initialize()override;
	void Finalize()override;
	void InitParam();
	void Write(const CB_TYPE::Kind cb);
	void BindVS(const VS_TYPE::Kind vs);
	void BindPS(const PS_TYPE::Kind ps);

	/*
	*	ShaderResource:
	*		TEX_MAIN,
	*		TEX_SCREEN,
	*		TEX_NEARSHADOW,
	*		TEX_MEDIUMSHADOW,
	*		TEX_FARSHADOW,
	*		TEX_TRILANAR_X,
	*		TEX_TRILANAR_Y,
	*		TEX_TRILANAR_Z,
	*		TEX_BUMP_X,
	*		TEX_BUMP_Y,
	*		TEX_BUMP_Z,
	*		TEX_CLIP,
	*		TEX_BLUR,
	*		TEX_GRAYSCALE,
	*/
	void SetTexture(ID3D11ShaderResourceView* pTex = nullptr, const ShaderResource::TEX_TYPE type = ShaderResource::TEX_TYPE::MAIN);
	
	void SetTexTilingOffset(const Vector2& scale, const Vector2& offset);
	void SetMultiplyColor(const Vector4& color);
	void SetGrayScaleAlpha(const float fAlpha);
	void SetTime(const float time);
	void SetTexSamplerWRAP();
	void SetTexSamplerCRAMP();

	void SetWorld(const DirectX::XMMATRIX& mWorld);

	void SetInstancingWorld(const UINT nSize);

	void SetCameraVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj);
	
	void SetLightVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj);
	
	void SetLightInfo(const ShaderResource::LightInfo& light);
	
	void SetCameraInfo(const ShaderResource::CameraInfo& camera);
	
protected:
	ShaderBuffer();
	~ShaderBuffer();

private:
	DirectX::XMFLOAT4X4 m_world;
	ShaderResource::CameraVP m_cameraVP;
	ShaderResource::Instancing m_Instancing;
	ShaderResource::LightVP m_lightVP;
	ShaderResource::CameraInfo m_cameraInfo;
	ShaderResource::LightInfo m_lightInfo;
	ShaderResource::TexSetting m_texSetting;
	int m_nSpotNext;

	std::vector<std::unique_ptr<VertexShader> > m_pVSList;
	std::vector<std::unique_ptr<PixelShader> > m_pPSList;
	std::vector<std::unique_ptr<ConstantBuffer> > m_pCBList;
};