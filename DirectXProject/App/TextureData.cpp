#include "TextureData.h"

void TextureData::Initialize()
{
}

void TextureData::Finalize()
{
	for (auto& itr : m_pTextureMap)
	{
		SAFE_RELEASE(itr.second);
	}
}

void TextureData::Load(std::string str, const std::string path)
{
	ID3D11ShaderResourceView* pTex;
	const std::string staticPath = "Assets/Texture/";
	std::string texPath = staticPath + path + str;
	int size = str.find(".");
	str.resize(size);
	LoadTextureFromFile(texPath.c_str(), &pTex);
	m_pTextureMap[str] = pTex;
}

ID3D11ShaderResourceView * TextureData::Get(std::string str)
{
	const int dot = str.find(".");
	if (dot != -1) //.png‚È‚Ç‚ÌŠg’£Žq‚ª‚Â‚¢‚Ä‚¢‚½‚Æ‚«íœ
	{
		str.resize(dot);
	}
	
	const auto itr = m_pTextureMap.find(str);
	if (itr != m_pTextureMap.end())
	{
		return itr->second;
	}
	return nullptr;
}
