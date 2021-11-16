#include "Renderer2D.h"
#include "Object.h"
#include "Image.h"

#include <System/Texture.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/Geometory.h>
#include <System/Input.h>

Renderer2D::Renderer2D()
	:m_isActive(true)
{

}


Renderer2D::~Renderer2D()
{
}


void Renderer2D::Init()
{
	m_Image.Init();
}

void Renderer2D::Uninit()
{
	m_Image.Uninit();
}


void Renderer2D::Update()
{
	m_Image.Update();
}


void Renderer2D::Draw()
{
	if (m_isActive)
	{
		m_Image.Draw();
		SHADER->SetWorld(MyMath::ConvertMatrix(
			Vector3(m_RectTransform.scale.x, m_RectTransform.scale.y, 0),
			Vector3(0, 0, m_RectTransform.rot),
			Vector3(SCREEN_WIDTH * 0.5f + m_RectTransform.pos.x, SCREEN_HEIGHT * 0.5f + m_RectTransform.pos.y, m_RectTransform.pos.z)));
		DrawPolygon();
	}
}

bool Renderer2D::MouseCollision()
{
	Vector2 mouse;
	mouse.x = GetMousePosX();
	mouse.y = GetMousePosY();
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