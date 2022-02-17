#pragma once
#include "Renderer.h"
#include <App/Image.h>

class FadeBase;

class Renderer2D : public Renderer
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
	Renderer2D() {}
	~Renderer2D() override {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	
	//void Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteStep::kind type)override {}
	void Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawStep::kind type)override;

	bool MouseCollision();
	
	inline void SetFadeAnimation(const std::weak_ptr<FadeBase> pFade) { m_pFadeAnimation = std::move(pFade); }

	inline const RectTransform& GetRectTransform() { return m_RectTransform; }
	inline void SetRectTransform(const RectTransform& rect) { m_RectTransform = rect; }


	Image m_Image;

private:
	std::weak_ptr<FadeBase> m_pFadeAnimation;
	RectTransform m_RectTransform;
	bool m_isActive;
};