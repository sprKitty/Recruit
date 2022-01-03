#include "Image.h"
#include <System/Clocker.h>
#include <App/TextureData.h>


void Image::Init()
{
	m_vTiling = 1;
	m_vOffset = 0;
	m_vMultiplyColor = 1;
	m_pTex = nullptr;
	m_isWRAP = true;
	m_type = ShaderResource::TEX_TYPE::MAIN;
}

void Image::Uninit()
{
}

void Image::Bind(const std::weak_ptr<ShaderBuffer>& pBuf)
{
	pBuf.lock()->SetTexTilingOffset(m_vTiling, m_vOffset);

	if (m_pTex)
		pBuf.lock()->SetTexturePS(m_pTex, m_type);
	else
		pBuf.lock()->SetTexturePS(nullptr, m_type);

	if (m_isWRAP)
		pBuf.lock()->SetTexSamplerWRAP();
	else
		pBuf.lock()->SetTexSamplerCRAMP();

	pBuf.lock()->SetMultiplyColor(m_vMultiplyColor);
}

void Image::SetTexturePS(const std::string str)
{
	m_pTex = TextureData::GetInstance().Get(str);
}
