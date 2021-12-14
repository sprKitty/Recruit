#pragma once
#include <System/ClassDesign/Singleton.h>
#include <System/Texture.h>
#include <map>


class TextureData : public Singleton<TextureData>
{
public:
	friend class Singleton<TextureData>;

public:
	void Initialize()override;
	void Finalize()override;

	void Load(std::string str, const std::string path = "");
	ID3D11ShaderResourceView* Get(std::string str);
protected:
	TextureData() {}
	~TextureData() {}

private:
	std::map<std::string, ID3D11ShaderResourceView*> m_pTextureMap;

};