#pragma once
#include "Image.h"
#include <App/Component/Component.h>

class Renderer2D : public Component
{
public:
struct RectTransform
{
	Vector2 scale;
	float rot;
	Vector3 pos;

	RectTransform() :scale(1), rot(0), pos(0) {}
	~RectTransform() {}
};
public:
	Renderer2D();
	~Renderer2D() override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	bool MouseCollision();
	
private:

	Image m_Image;
	RectTransform m_RectTransform;
	bool m_isActive;
	inline RectTransform GetRectTransform() { return m_RectTransform; }
	inline RectTransform& AdderssRectTransform() { return m_RectTransform; }
	inline void SetRectTransform(RectTransform rect) { m_RectTransform = rect; }
};