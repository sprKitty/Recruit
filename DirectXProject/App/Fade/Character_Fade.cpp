#include "Character_Fade.h"
#include <MyMath.h>

void Character_Fade::Init()
{
	m_pImage.reset(new Image);
	m_pImage->SetTexture("a.png");
	m_pImage->SetTexType(ShaderResource::TEX_TYPE::GRAYSCALE);
	m_isIn = false;
	m_isOut = false;
	m_fInOutSpeed = 0.5f;
	m_fTime = 0.f;
}

void Character_Fade::Uninit()
{
	m_pImage.reset();
}

void Character_Fade::Bind(const std::weak_ptr<ShaderBuffer> pSB)
{
	if (!m_isIn && !m_isOut)return;	// フェードインもフェードアウトも実行される状態ではなければスキップ
	if (pSB.expired())return;

	pSB.lock()->SetTime(MyMath::EaseInOutCubic(m_fInOut));
	m_pImage->BindTex(pSB);
}
