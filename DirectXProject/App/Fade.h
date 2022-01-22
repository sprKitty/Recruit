#pragma once
#include <App/Component/Object.h>
#include <App/Component/Renderer/Renderer2D.h>
#include <Shader/ShaderBuffer.h>


class Fade
{
public:
	Fade() {}
	~Fade() {}

	void Init();
	void Uninit();
	const bool Update();
	void Draw(const std::weak_ptr<ShaderBuffer> pSB);

	void StartFadeIn();
	void StartFadeOut();
private:
	void InUpdate();
	void OutUpdate();

private:
	bool m_isIn;
	bool m_isOut;
	Object::OWNER_OBJ m_pObject;
	std::weak_ptr<Renderer2D> m_pRenderer2D;
	float m_fInOutSpeed;
	float m_fSpeed;
	float m_fInOut;
};