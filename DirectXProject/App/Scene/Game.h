#pragma once
#include <App/Scene/SceneBase.h>


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
	std::shared_ptr<Mouse> m_pMouse;
	std::shared_ptr<GameKeyBind> m_pKeyBind;
	std::shared_ptr<MessageWindow> m_pMessageWindow;
	int m_nMaxObjectNum;

};