#include "Image.h"
#include <System/Clocker.h>

void Image::Init()
{
	m_vTiling = 1;
	m_vMultiplyColor = 1;
	m_pTex = nullptr;
	m_isWRAP = true;
	m_type = ShaderResource::TEX_TYPE::MAIN;
}

void Image::Uninit()
{
	SAFE_RELEASE(m_pTex);
}

void Image::Bind()
{
	ShaderBuffer::GetInstance().SetTexTilingOffset(m_vTiling, m_vOffset);

	if (m_pTex)
		ShaderBuffer::GetInstance().SetTexture(m_pTex, m_type);
	else
		ShaderBuffer::GetInstance().SetTexture(nullptr, m_type);

	if (m_isWRAP)
		ShaderBuffer::GetInstance().SetTexSamplerWRAP();
	else
		ShaderBuffer::GetInstance().SetTexSamplerCRAMP();

	ShaderBuffer::GetInstance().SetMultiplyColor(m_vMultiplyColor);
}
