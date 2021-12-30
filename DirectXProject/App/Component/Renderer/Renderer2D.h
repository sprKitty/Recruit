#pragma once
#include <App/Component/Image.h>
#include <App/Component/Renderer/Renderer.h>

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
	Renderer2D();
	~Renderer2D() override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	
	void Write(const std::weak_ptr<ShaderBuffer> pBuf, const WriteType::kind type)override {}
	void Draw(const std::weak_ptr<ShaderBuffer> pBuf, const DrawType::kind type = DrawType::UI)override;

	bool MouseCollision();
	
	inline const RectTransform& GetRectTransform() { return m_RectTransform; }
	inline void SetRectTransform(const RectTransform& rect) { m_RectTransform = rect; }

	Image m_Image;

private:
	RectTransform m_RectTransform;
	bool m_isActive;
};