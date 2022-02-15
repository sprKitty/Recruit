#include "Scene_Level_Fade.h"

const char* g_pFadeInfoFile = "Assets/csv/fadeInfo.csv";

void Scene_Level_Fade::Init()
{
	m_pObject.reset(new Object());
	m_pRenderer2D = m_pObject->AddComponent<Renderer2D>();
	if (!m_pRenderer2D.expired())
	{
		m_pRenderer2D.lock()->EnableDraw(DrawType::UI_FADE);
	}
	LoadFiles loadFiles;
	STRINGMAP strMap;
	Renderer2D::RectTransform rectTransform;
	rectTransform.scale = { SCREEN_WIDTH,SCREEN_HEIGHT };

	m_isIn = false;
	m_isOut = false;
	inOutSpeed.set(0.5f);
	time.set(0.0f);
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
			inOutSpeed.set(std::stof(itr.second[0].c_str()));
		}
	}
}

void Scene_Level_Fade::Uninit()
{
	m_pObject.reset();
}

void Scene_Level_Fade::Bind(const std::weak_ptr<ShaderBuffer> pSB)
{
	if (!m_isIn && !m_isOut)return;	// フェードインもフェードアウトも実行される状態ではなければスキップ
	if (pSB.expired())return;
	if (m_pRenderer2D.expired())return;

	pSB.lock()->SetTime(MyMath::EaseInOutCubic(m_fInOut));
	m_pRenderer2D.lock()->Draw(pSB, DrawType::UI_FADE);
}
