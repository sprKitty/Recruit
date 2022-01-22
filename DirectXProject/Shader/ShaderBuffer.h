#pragma once
#include <DirectXMath.h>
#include <System/DirectX.h>
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
		CAMERADEPTH,
		GAUSSIANBLUR,
		WATERREFLECTION,
		GRASSMOVE,

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
		CAMERADEPTH,
		GAUSSIANBLUR,
		KAWASEBLOOM,
		MIX,
		DOF,
		WATERREFLECTION,
		MAINSCREEN,
		FADE,
		
		MAX
	};
}

namespace CB_TYPE
{
	enum Kind
	{
		INSTANCING_WORLD = 0,
		CAMEAR_VP,
		LIGHT_VP,
		CAMERA_INFO,
		LIGHT_INFO,
		TEX_SETTING,
		POSTEFFECT,
		MAX
	};
}

namespace ShaderResource
{
	static constexpr int VPBUFFER_SIZE = 2;

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
		WATER,
		WATER_HEIGHT,
		WATER_BUMP,
		DEPTH_OF_SAHDOW,
		DEPTH_OF_FIELD,
		BUMP,
		GRAYSCALE,
		BLURX,
		BLURY,
		KAWASEX_1,
		KAWASEY_1,
		KAWASEX_2,
		KAWASEY_2,
		EFFECT,
	};

	struct ViewProjection
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};

	struct CameraVP
	{
		ViewProjection info[VPBUFFER_SIZE];
	};

	struct LightVP
	{
		ViewProjection info[VPBUFFER_SIZE];
	};

	struct CameraInfo
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 vp;
	};

	struct LightInfo
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 decay;
		DirectX::XMFLOAT4 color;
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
		float fGameStartCnt;
	};

	struct PostEffect
	{
		Vector4 blur[8];
	};

};

class ShaderBuffer
{
public:
	ShaderBuffer();
	~ShaderBuffer();

	void Initialize();
	void Finalize();
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
	void SetTextureVS(ID3D11ShaderResourceView* pTex = nullptr, const ShaderResource::TEX_TYPE type = ShaderResource::TEX_TYPE::MAIN);
	void SetTexturePS(ID3D11ShaderResourceView* pTex = nullptr, const ShaderResource::TEX_TYPE type = ShaderResource::TEX_TYPE::MAIN);
	
	void SetTexTilingOffset(const Vector2& scale, const Vector2& offset);
	void SetMultiplyColor(const Vector4& color);
	void SetGrayScaleAlpha(const float fAlpha);
	void SetTime(const float time);
	void SetGameStartCnt(const float time);
	void SetTexSamplerWRAP();
	void SetTexSamplerCRAMP();

	void SetWorld(const DirectX::XMMATRIX& mWorld);

	void SetInstancingWorld(const Vector3& vScl, const Vector3& vRot, const std::vector<Vector3>& posList);

	void SetCameraVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj, const int num);
	
	void SetLightVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj, const int num);
	
	void SetLightInfo(const ShaderResource::LightInfo& light);
	
	void SetCameraInfo(const ShaderResource::CameraInfo& camera);
	
	void SetPostEffectInfo(const ShaderResource::PostEffect& post);


private:
	static const int INSTANCING_MAX = 1024;

	DirectX::XMFLOAT4X4 m_instancingWorld[INSTANCING_MAX];
	ShaderResource::CameraVP m_cameraVP;
	ShaderResource::LightVP m_lightVP;
	ShaderResource::CameraInfo m_cameraInfo;
	ShaderResource::LightInfo m_lightInfo;
	ShaderResource::TexSetting m_texSetting;
	ShaderResource::PostEffect m_postEffect;
	int m_nSpotNext;

	std::vector<std::unique_ptr<VertexShader> > m_pVSList;
	std::vector<std::unique_ptr<PixelShader> > m_pPSList;
	std::vector<std::unique_ptr<ConstantBuffer> > m_pCBList;
};