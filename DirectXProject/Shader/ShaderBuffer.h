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

enum class VS_TYPE
{
	NORMAL,
	ANIMATION,
	LIGHTDEPTH,
	CAMERADEPTH,
	WATER,
	OUTLINE,
	DUMMY,

	MAX
};

enum class PS_TYPE
{
	NORMAL,
	BUFFER,
	COLOR,
	SCOPE,
	BLUR,
	TOON,
	TOONSHADOW,
	WORLD,
	LIGHTDEPTH,
	CAMERADEPTH,
	FADE,
	WATER,

	MAX
};

enum class CB_TYPE
{
	WORLD = 0,
	V_REGIST1,
	V_REGIST2,
	P_REGIST0,
	P_REGIST1,
	P_REGIST2,

	MAX
};

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
		TRILANAR_X,
		TRILANAR_Y,
		TRILANAR_Z,
		BUMP_X,
		BUMP_Y,
		BUMP_Z,
		CLIP,
		BLUR,
		GRAYSCALE,
		TOONMAP,
		WORLD,
	};


	struct CameraInfo
	{
		DirectX::XMFLOAT4 pos;
	};

	struct LightInfo
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT3 direction;
		float fSoft;
		DirectX::XMFLOAT4 decay;
		DirectX::XMFLOAT4 color;
	};

	struct VSRegist1
	{
		DirectX::XMFLOAT4X4 camView;
		DirectX::XMFLOAT4X4 camProj;
		DirectX::XMFLOAT4X4 parallelLightView;
		DirectX::XMFLOAT4X4 parallelLightProj;
	};

	struct VSRegist2
	{
		DirectX::XMFLOAT4X4 bone[250];
	};

	struct PSRegist0
	{
		CameraInfo camInfo; // カメラの詳細
		LightInfo parallelLI; // 並行光源の詳細
	};

	struct PSRegist1
	{
		DirectX::XMFLOAT4 color;
		int nBlur;
		int nWidth;
		int nHeight;
		int nDummy;
	};

	// テクスチャの設定
	struct PSRegist2
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
	void Write(CB_TYPE cb);
	void BindVS(VS_TYPE vs);
	void BindPS(PS_TYPE ps);

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
	void SetTexture(ID3D11ShaderResourceView* pTex = nullptr, ShaderResource::TEX_TYPE type = ShaderResource::TEX_TYPE::MAIN);
	
	void SetTexTilingOffset(Vector2& scale, Vector2& offset);
	void SetMultiplyColor(Vector4& color);
	void SetGrayScaleAlpha(float fAlpha);
	void SetTime(float& time);
	void SetTexSamplerWRAP();
	void SetTexSamplerCRAMP();

	void SetWorld(const DirectX::XMMATRIX& mWorld);

	void SetCameraVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj);
	
	void SetParallelLightVP(const DirectX::XMMATRIX& mView, const DirectX::XMMATRIX& mProj);
	
	void SetAnimeBone(const DirectX::XMFLOAT4X4* pBone);
	
	void SetColor(const DirectX::XMFLOAT4& color);
	
	void SetParallelLightInfo(const ShaderResource::LightInfo& light);
	
	void SetCameraInfo(const ShaderResource::CameraInfo& camera);
	
	void SetBlur(bool flg);

protected:
	ShaderBuffer();
	~ShaderBuffer();

private:
	void SetToonMap(ID3D11ShaderResourceView* pMap);

	DirectX::XMFLOAT4X4 m_world;
	ShaderResource::VSRegist1 m_vs1;
	ShaderResource::VSRegist2 m_vs2;
	ShaderResource::PSRegist0 m_ps0;
	ShaderResource::PSRegist1 m_ps1;
	ShaderResource::PSRegist2 m_ps2;
	ID3D11ShaderResourceView* m_pMap;
	int m_nSpotNext;

	std::vector<std::unique_ptr<VertexShader> > m_pVSList;
	std::vector<std::unique_ptr<PixelShader> > m_pPSList;
	std::vector<std::unique_ptr<ConstantBuffer> > m_pCBList;
};