#pragma once
#include "FadeBase.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer/Renderer2D.h>

class Scene_Level_Fade final : public FadeBase
{
public:
	Scene_Level_Fade() {}
	virtual ~Scene_Level_Fade()override {}

	void Init()override;
	virtual void Uninit()override;
	void Bind(const std::weak_ptr<ShaderBuffer> pSB)override;

private:
	Object::OWNER_OBJ m_pObject;
	std::weak_ptr<Renderer2D> m_pRenderer2D;
};