#include "Fade.h"
#include <System/LoadFiles.h>
#include <System/Clocker.h>

const char* g_pFadeInfoFile = "Assets/csv/fadeInfo.csv";

void Fade::Init()
{
	m_pObject.reset(new Object());
	m_pRenderer2D = m_pObject->AddComponent<Renderer2D>();
	LoadFiles loadFiles;
	STRINGMAP strMap;
	Renderer2D::RectTransform rectTransform;
	rectTransform.scale = { SCREEN_WIDTH,SCREEN_HEIGHT };

	m_isIn = false;
	m_isOut = false;
	m_fInOutSpeed = 0.5f;
	m_fSpeed = 0.0f;
	m_pRenderer2D.lock()->SetRectTransform(rectTransform);
	m_pRenderer2D.lock()->m_Image.SetTexType(ShaderResource::TEX_TYPE::GRAYSCALE);
	strMap = loadFiles.CSV(g_pFadeInfoFile);
	for (const auto& itr : strMap)
	{
		if (itr.first == "1ファイル名")
		{
			m_pRenderer2D.lock()->m_Image.SetTexture(itr.second[0].c_str());
		}
		else if (itr.first == "2フェードインアウトスピード")
		{
			m_fInOutSpeed = std::stof(itr.second[0].c_str());
		}
	}
}

void Fade::Uninit()
{
	m_pObject.reset();
}

const bool Fade::Update()
{
	if (m_isIn)
	{
		if (m_fSpeed >= m_fInOutSpeed)
		{
			m_isIn = false;
			m_fSpeed = m_fInOutSpeed;
			return false;
		}
		InUpdate();
		return true;
	}
	else if (m_isOut)
	{
		if (m_fSpeed <= 0.0f)
		{
			m_isOut = false;
			m_fSpeed = 0.0f;
			return false;
		}
		OutUpdate();
		return true;
	}
	return false;
}

void Fade::Draw(const std::weak_ptr<ShaderBuffer> pSB)
{
	if (!m_isIn && !m_isOut)return;	// フェードインもフェードアウトも実行される状態ではなければスキップ
	if (pSB.expired())return;
	if (m_pRenderer2D.expired())return;
		
	pSB.lock()->SetTime(MyMath::EaseInOutCubic(m_fInOut));
	pSB.lock()->BindVS(VS_TYPE::NORMAL);
	pSB.lock()->BindPS(PS_TYPE::FADE);
	m_pRenderer2D.lock()->Draw(pSB);
}

void Fade::StartFadeIn()
{
	m_isIn = true;
	m_isOut = false;
}

void Fade::StartFadeOut()
{
	m_isIn = false;
	m_isOut = true;
}

void Fade::InUpdate()
{
	m_fSpeed += Clocker::GetInstance().GetFrameTime();
	m_fInOut = m_fSpeed / m_fInOutSpeed;
}

void Fade::OutUpdate()
{
	m_fSpeed -= Clocker::GetInstance().GetFrameTime();
	m_fInOut = m_fSpeed / m_fInOutSpeed;
}
