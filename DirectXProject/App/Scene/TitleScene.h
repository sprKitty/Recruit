#pragma once
#include <App/Scene/SceneBase.h>


class Mouse;

class TitleScene : public SceneBase
{
public:
	TitleScene() {}
	~TitleScene()override {}

	void Init()override;
	void Uninit()override;
	const Scene_Type::kind Update()override;
	void Draw()override;

private:
	Object::WORKER_OBJ m_pPlayer;
	std::shared_ptr<Mouse> m_pMouse;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Camera> m_pReflectionCamera;
	std::shared_ptr<Camera> m_pKawaseBloom;
	std::shared_ptr<Camera> m_pBlurXCamera;
	std::shared_ptr<Camera> m_pBlurYCamera;

	std::shared_ptr<Light> m_pLight;
	bool m_isNextScene;
};