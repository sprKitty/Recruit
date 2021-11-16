#pragma once
#include <App/Scene/Scene.h>

class Renderer2D;

class Title:public Scene
{
public:
	Title();
	~Title()override;

	void Start()override;
	void Init()override;
	void Uninit()override;
	Scene_Type Update(Scene_Type type)override;
	void Draw()override;
private:
	Renderer2D* m_pAsobu;
	Renderer2D* m_pSettei;
};