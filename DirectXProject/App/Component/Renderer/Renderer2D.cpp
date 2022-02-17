#include "Renderer2D.h"
#include <App/Component/Object.h>
#include <App/Image.h>
#include <App/Fade/FadeBase.h>
#include <System/Texture.h>
#include <System/Geometory.h>
#include <System/Input.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>


void Renderer2D::Init()
{
	m_Image.Init();
	m_isActive = true;

	m_isDrawType.resize(DrawType::MAX);
	for (bool itr : m_isDrawType)
	{
		itr = false;
	}
}

void Renderer2D::Uninit()
{
	m_Image.Uninit();
}


void Renderer2D::Update()
{
}

void Renderer2D::Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawType::kind type)
{
	if (type != DrawType::MAX)
	{
		if (!m_isDrawType[type])return;
	}
	
	switch (type)
	{
	case DrawType::UI_NORMAL:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::CHARACTER);
		break;
	
	case DrawType::UI_MAGIC:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::CHARACTER);
		break;
	
	case DrawType::UI_FADE:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::FADE);
		break;
	case DrawType::WORLD_OF_NORMAL:
	case DrawType::WORLD_OF_CHARACTER:
	case DrawType::WORLD_OF_EFFECT:
	case DrawType::WORLD_OF_WATER:
	case DrawType::WORLD_OF_TRIPLANAR:
	case DrawType::MAX:
	default:
		return;
	}

	if (pBuf.expired())return;

	if (m_isActive)
	{
		if (!m_pFadeAnimation.expired())
		{
			m_pFadeAnimation.lock()->Bind(pBuf);
		}
		else
		{
			pBuf.lock()->SetTexturePS(nullptr, ShaderResource::TEX_TYPE::GRAYSCALE);
		}

		m_Image.Bind(pBuf);

		pBuf.lock()->SetWorld(MyMath::ConvertMatrix(
			Vector3(m_RectTransform.scale.x, m_RectTransform.scale.y, 0),
			Vector3(0, 0, m_RectTransform.rot),
			Vector3(SCREEN_WIDTH * 0.5f + m_RectTransform.pos.x, SCREEN_HEIGHT * 0.5f + m_RectTransform.pos.y, m_RectTransform.pos.z)));
		Geometory::GetInstance().DrawPolygon();
	}
}

bool Renderer2D::MouseCollision()
{
	Vector2 mouse;
	mouse.x = static_cast<float>(GetMousePosX());
	mouse.y = static_cast<float>(GetMousePosY());
	Vector2 pos = Vector2(m_RectTransform.pos.x, m_RectTransform.pos.y);
	Vector2 scale = m_RectTransform.scale * 0.5f;
	mouse.x -= HALFSCREEN_WIDTH;
	mouse.y -= HALFSCREEN_HEIGHT;
	if (mouse.x > pos.x - scale.x &&
		mouse.x < pos.x + scale.x &&
		mouse.y > pos.y - scale.y &&
		mouse.y < pos.y + scale.y)
		return true;
	return false;
}