#pragma once
#include "Component.h"
#include <System/Texture.h>
#include <Shader/ShaderBuffer.h>

class Image
{
public:
	Image() { Init(); }
	~Image() { Uninit(); }

	void Init();
	void Uninit();
	void Bind();
	
	void SetTexture(const std::string str);
	/*
	*	ShaderResource::
	*		TEX_MAIN = 0,
	*		TEX_SCREEN = 1,
	*		TEX_NEARSHADOW = 2,
	*		TEX_MEDIUMSHADOW = 3,
	*		TEX_FARSHADOW = 4,
	*		TEX_TRILANAR_X = 5,
	*		TEX_TRILANAR_Y = 6,
	*		TEX_TRILANAR_Z = 7,
	*		TEX_BUMP_X = 8,
	*		TEX_BUMP_Y = 9,
	*		TEX_BUMP_Z = 10,
	*		TEX_CLIP = 11,
	*		TEX_BLUR = 12,
	*		TEX_GRAYSCALE = 13,
	*/	
	inline void SetTexType(const ShaderResource::TEX_TYPE type) 
	{
		m_type = type;
	}

	inline const ID3D11ShaderResourceView* GetTexture() 
	{ 
		return m_pTex; 
	}

	inline void TexSamplerWRAP()
	{ 
		m_isWRAP = true;
	}
	inline void TexSamplerCRAMP() 
	{ 
		m_isWRAP = false; 
	}

	Vector2 m_vTiling;
	Vector2 m_vOffset;
	Vector4 m_vMultiplyColor;

private:
	ID3D11ShaderResourceView* m_pTex;
	ShaderResource::TEX_TYPE m_type;
	bool m_isWRAP;
};