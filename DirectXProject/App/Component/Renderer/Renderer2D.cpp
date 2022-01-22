#include "Renderer2D.h"
#include <App/Component/Object.h>
#include <App/Component/Image.h>

#include <System/Texture.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/Geometory.h>
#include <System/Input.h>


void Renderer2D::Init()
{
	m_Image.Init();
	m_isActive = true;
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
	if (type != DrawType::UI)return;
	if (pBuf.expired())return;

	if (m_isActive)
	{
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
	Vector2 pos = Vector2(m_RectTransform.pos.x - DEVISIONX * mouse.x, m_RectTransform.pos.y - DEVISIONY * mouse.y);
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