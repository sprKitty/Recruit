#include "MessageWindow.h"
#include <Defines.h>
#include <App/Component/Renderer/Renderer2D.h>
#include <System/LoadFiles.h>


float MessageWindow::m_fShowSpeed = 0.2f;
std::map<std::string, Vector2> MessageWindow::m_OffsetMap;

void MessageWindow::Initialize()
{
	m_pObject.reset(new Object());

	m_pWindow = m_pObject->AddComponent<Renderer2D>();
	Renderer2D::RectTransform rect;
	rect.pos = Vector3(static_cast<float>(0), static_cast<float>(SCREEN_HEIGHT) * 0.35f, 10);
	rect.scale = Vector2(static_cast<float>(SCREEN_WIDTH) * 0.7f, static_cast<float>(SCREEN_HEIGHT) * 0.25f);
	m_pWindow.lock()->SetRectTransform(rect);
	m_pWindow.lock()->m_Image.SetTexture("textframe.png");

	m_pHiragana = m_pObject->AddComponent<Renderer2D>();
	rect.pos = Vector3(static_cast<float>(0), static_cast<float>(SCREEN_HEIGHT) * 0.4f, 9);
	rect.scale = Vector2(static_cast<float>(SCREEN_WIDTH) * 0.7f, static_cast<float>(SCREEN_HEIGHT) * 0.15f);
	m_pHiragana.lock()->SetRectTransform(rect);
	m_pHiragana.lock()->m_Image.m_vTiling = 0.1f;
	m_pHiragana.lock()->m_Image.SetTexture("hiragana.png");
	
	LoadFiles lf;
	STRINGMAP sm = lf.CSV("Assets/csv/message.csv");
	if (!sm["showspeed"][0].empty())
	{
		m_fShowSpeed = std::stof(sm["showspeed"][0]);
	}
}

void MessageWindow::Finalize()
{
	m_pObject.reset();
}

void MessageWindow::Update()
{
}

void MessageWindow::Bind(const std::vector<Vector2>& vTitle, const std::vector<Vector2>& vMain, const int nFontSize, const int nAnimation)
{
	m_StackMsgInfo.nMainFontSize = nFontSize;
	std::vector<Vector2> vMainMessage = vMain;

	if (nAnimation == -1)
	{
		m_StackMsgInfo.vTitleOffsets = vTitle;
		m_StackMsgInfo.vMainOffsets = vMainMessage;
	}
	else
	{
		m_StackMsgInfo.vTitleOffsets = vTitle;
		vMainMessage.resize(nAnimation);
		m_StackMsgInfo.vMainOffsets = vMainMessage;
	}
}

void MessageWindow::Draw()
{
	if (!m_pHiragana.expired())
	{
		if (!m_StackMsgInfo.vMainOffsets.empty() && !m_StackMsgInfo.vTitleOffsets.empty())
		{
			if (!m_pWindow.expired())
			{
				m_pWindow.lock()->Draw();
			}

			Renderer2D::RectTransform rect = m_pHiragana.lock()->GetRectTransform();
			float fSize = 32.0f / static_cast<float>(SCREEN_HEIGHT) * 500.0f;
			rect.scale.x = static_cast<float>(fSize);
			rect.scale.y = static_cast<float>(fSize);
			for (int i = 0; i < m_StackMsgInfo.vTitleOffsets.size(); ++i)
			{
				rect.pos.x = static_cast<float>(SCREEN_WIDTH) * -0.3f + fSize * i;
				rect.pos.y = static_cast<float>(SCREEN_HEIGHT) * 0.25f;
				m_pHiragana.lock()->SetRectTransform(rect);
				m_pHiragana.lock()->m_Image.m_vOffset = m_StackMsgInfo.vTitleOffsets[i];
				m_pHiragana.lock()->Draw();
			}

			fSize = static_cast<float>(m_StackMsgInfo.nMainFontSize) / static_cast<float>(SCREEN_HEIGHT) * 500.0f;
			rect.scale.x = static_cast<float>(fSize);
			rect.scale.y = static_cast<float>(fSize);
			for (int i = 0; i < m_StackMsgInfo.vMainOffsets.size(); ++i)
			{
				rect.pos.x = static_cast<float>(SCREEN_WIDTH) * -0.3f + fSize * i;
				rect.pos.y = static_cast<float>(SCREEN_HEIGHT) * 0.35f;
				m_pHiragana.lock()->SetRectTransform(rect);
				m_pHiragana.lock()->m_Image.m_vOffset = m_StackMsgInfo.vMainOffsets[i];
				m_pHiragana.lock()->Draw();
			}
		}
		m_StackMsgInfo.vTitleOffsets.clear();
		m_StackMsgInfo.vMainOffsets.clear();
	}
}

void MessageWindow::CreateOffsetMap()
{
	m_OffsetMap["あ"] = Vector2(0.0f, 0.0f);
	m_OffsetMap["い"] = Vector2(0.1f, 0.0f);
	m_OffsetMap["う"] = Vector2(0.2f, 0.0f);
	m_OffsetMap["え"] = Vector2(0.3f, 0.0f);
	m_OffsetMap["お"] = Vector2(0.4f, 0.0f);

	m_OffsetMap["か"] = Vector2(0.0f, 0.1f);
	m_OffsetMap["き"] = Vector2(0.1f, 0.1f);
	m_OffsetMap["く"] = Vector2(0.2f, 0.1f);
	m_OffsetMap["け"] = Vector2(0.3f, 0.1f);
	m_OffsetMap["こ"] = Vector2(0.4f, 0.1f);

	m_OffsetMap["さ"] = Vector2(0.0f, 0.2f);
	m_OffsetMap["し"] = Vector2(0.1f, 0.2f);
	m_OffsetMap["す"] = Vector2(0.2f, 0.2f);
	m_OffsetMap["せ"] = Vector2(0.3f, 0.2f);
	m_OffsetMap["そ"] = Vector2(0.4f, 0.2f);

	m_OffsetMap["た"] = Vector2(0.0f, 0.3f);
	m_OffsetMap["ち"] = Vector2(0.1f, 0.3f);
	m_OffsetMap["つ"] = Vector2(0.2f, 0.3f);
	m_OffsetMap["て"] = Vector2(0.3f, 0.3f);
	m_OffsetMap["と"] = Vector2(0.4f, 0.3f);

	m_OffsetMap["な"] = Vector2(0.0f, 0.4f);
	m_OffsetMap["に"] = Vector2(0.1f, 0.4f);
	m_OffsetMap["ぬ"] = Vector2(0.2f, 0.4f);
	m_OffsetMap["ね"] = Vector2(0.3f, 0.4f);
	m_OffsetMap["の"] = Vector2(0.4f, 0.4f);

	m_OffsetMap["は"] = Vector2(0.0f, 0.5f);
	m_OffsetMap["ひ"] = Vector2(0.1f, 0.5f);
	m_OffsetMap["ふ"] = Vector2(0.2f, 0.5f);
	m_OffsetMap["へ"] = Vector2(0.3f, 0.5f);
	m_OffsetMap["ほ"] = Vector2(0.4f, 0.5f);

	m_OffsetMap["ま"] = Vector2(0.0f, 0.6f);
	m_OffsetMap["み"] = Vector2(0.1f, 0.6f);
	m_OffsetMap["む"] = Vector2(0.2f, 0.6f);
	m_OffsetMap["め"] = Vector2(0.3f, 0.6f);
	m_OffsetMap["も"] = Vector2(0.4f, 0.6f);

	m_OffsetMap["や"] = Vector2(0.0f, 0.7f);
	m_OffsetMap["ゆ"] = Vector2(0.2f, 0.7f);
	m_OffsetMap["よ"] = Vector2(0.4f, 0.7f);

	m_OffsetMap["ら"] = Vector2(0.0f, 0.8f);
	m_OffsetMap["り"] = Vector2(0.1f, 0.8f);
	m_OffsetMap["る"] = Vector2(0.2f, 0.8f);
	m_OffsetMap["れ"] = Vector2(0.3f, 0.8f);
	m_OffsetMap["ろ"] = Vector2(0.4f, 0.8f);

	m_OffsetMap["わ"] = Vector2(0.0f, 0.9f);
	m_OffsetMap["を"] = Vector2(0.2f, 0.9f);
	m_OffsetMap["ん"] = Vector2(0.4f, 0.9f);


	m_OffsetMap["ヴ"] = Vector2(0.7f, 0.0f);

	m_OffsetMap["が"] = Vector2(0.5f, 0.1f);
	m_OffsetMap["ぎ"] = Vector2(0.6f, 0.1f);
	m_OffsetMap["ぐ"] = Vector2(0.7f, 0.1f);
	m_OffsetMap["げ"] = Vector2(0.8f, 0.1f);
	m_OffsetMap["ご"] = Vector2(0.9f, 0.1f);

	m_OffsetMap["ざ"] = Vector2(0.5f, 0.2f);
	m_OffsetMap["じ"] = Vector2(0.6f, 0.2f);
	m_OffsetMap["ず"] = Vector2(0.7f, 0.2f);
	m_OffsetMap["ぜ"] = Vector2(0.8f, 0.2f);
	m_OffsetMap["ぞ"] = Vector2(0.9f, 0.2f);

	m_OffsetMap["だ"] = Vector2(0.5f, 0.3f);
	m_OffsetMap["ぢ"] = Vector2(0.6f, 0.3f);
	m_OffsetMap["づ"] = Vector2(0.7f, 0.3f);
	m_OffsetMap["で"] = Vector2(0.8f, 0.3f);
	m_OffsetMap["ど"] = Vector2(0.9f, 0.3f);

	m_OffsetMap["ば"] = Vector2(0.5f, 0.4f);
	m_OffsetMap["び"] = Vector2(0.6f, 0.4f);
	m_OffsetMap["ぶ"] = Vector2(0.7f, 0.4f);
	m_OffsetMap["べ"] = Vector2(0.8f, 0.4f);
	m_OffsetMap["ぼ"] = Vector2(0.9f, 0.4f);

	m_OffsetMap["ぱ"] = Vector2(0.5f, 0.5f);
	m_OffsetMap["ぴ"] = Vector2(0.6f, 0.5f);
	m_OffsetMap["ぷ"] = Vector2(0.7f, 0.5f);
	m_OffsetMap["ぺ"] = Vector2(0.8f, 0.5f);
	m_OffsetMap["ぽ"] = Vector2(0.9f, 0.5f);

	m_OffsetMap["ぁ"] = Vector2(0.5f, 0.6f);
	m_OffsetMap["ぃ"] = Vector2(0.6f, 0.6f);
	m_OffsetMap["ぅ"] = Vector2(0.7f, 0.6f);
	m_OffsetMap["ぇ"] = Vector2(0.8f, 0.6f);
	m_OffsetMap["ぉ"] = Vector2(0.9f, 0.6f);

	m_OffsetMap["ゃ"] = Vector2(0.5f, 0.7f);
	m_OffsetMap["ゅ"] = Vector2(0.7f, 0.7f);
	m_OffsetMap["ょ"] = Vector2(0.9f, 0.7f);

	m_OffsetMap["っ"] = Vector2(0.7f, 0.8f);

	m_OffsetMap["！"] = Vector2(0.5f, 0.9f);
	m_OffsetMap["？"] = Vector2(0.6f, 0.9f);
	m_OffsetMap["・"] = Vector2(0.7f, 0.9f);
	m_OffsetMap["ー"] = Vector2(0.8f, 0.9f);
}
