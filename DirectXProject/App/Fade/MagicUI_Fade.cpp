#include "MagicUI_Fade.h"
#include <MyMath.h>

void MagicUI_Fade::Init()
{
	m_pImage.reset(new Image);
	m_pImage->SetTexture("hpbar.png");
	m_pImage->SetTexType(ShaderResource::TEX_TYPE::GRAYSCALE);
	m_isIn = true;
	m_isOut = false;
	inOutSpeed.set(0.5f);
	time.set(0.f);
}

void MagicUI_Fade::Uninit()
{
	m_pImage.reset();
}

void MagicUI_Fade::Bind(const std::weak_ptr<ShaderBuffer> pSB)
{
	if (!m_isIn && !m_isOut)return;	// フェードインもフェードアウトも実行される状態ではなければスキップ
	if (pSB.expired())return;

	m_fInOut = time.get() / inOutSpeed.get();
	pSB.lock()->SetTime(1.f - m_fInOut);
	m_pImage->BindTex(pSB);
}
