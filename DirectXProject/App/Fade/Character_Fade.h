#pragma once
#include "FadeBase.h"
#include <App/Image.h>

class Character_Fade final : public FadeBase
{
public:
	Character_Fade() {}
	virtual ~Character_Fade()override {}

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Bind(const std::weak_ptr<ShaderBuffer> pSB)override;

private:
	std::shared_ptr<Image> m_pImage;
};
