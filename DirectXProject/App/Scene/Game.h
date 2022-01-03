#pragma once
#include <App/Scene/SceneBase.h>
#include <App/Level/LevelBase.h>

class Mouse;
class GameKeyBind;
class MessageWindow;

class Game : public SceneBase
{
public:
	Game() {}
	~Game()override {}
	
	void Init()override;
	void Uninit()override;
	Scene_Type::kind Update()override;
	void Draw()override;

private:
	Object::WORKER_OBJ m_pPlayer;
	std::unique_ptr<LevelBase> m_pLevel;
	std::shared_ptr<Mouse> m_pMouse;
	std::shared_ptr<GameKeyBind> m_pKeyBind;
	std::shared_ptr<MessageWindow> m_pMessageWindow;
	std::shared_ptr<Camera> m_pCameraDepth;
	std::shared_ptr<Camera> m_pReflectionCamera;
	std::shared_ptr<Camera> m_pKawaseBloom;
	std::shared_ptr<Camera> m_pBlurXCamera;
	std::shared_ptr<Camera> m_pBlurYCamera;
	std::shared_ptr<Camera> m_pBlurXCameraHalf;
	std::shared_ptr<Camera> m_pBlurYCameraHalf;
	std::shared_ptr<Camera> m_pBlurXCameraHalfHalf;
	std::shared_ptr<Camera> m_pBlurYCameraHalfHalf;
	std::shared_ptr<Camera> m_pBlurXCameraHalfHalfHalf;
	std::shared_ptr<Camera> m_pBlurYCameraHalfHalfHalf;
	std::shared_ptr<Camera> m_pEffectCamera;
	int m_nMaxObjectNum;

};