#pragma once
#include <System/ClassDesign/Singleton.h>
#include <System/DirectX.h>
#include <App/Scene/SceneBase.h>
#include <App/Fade/FadeBase.h>
#include <App/ViewPoint/ViewPoint.h>

class ShaderBuffer;

class SceneMgr : public Singleton<SceneMgr>
{
public:
	friend class Singleton<SceneMgr>;

public:
	void Initialize()override {}
	void Finalize()override;
	HRESULT Init(HWND hWnd, UINT width, UINT height);
	void Update();
	void Draw();

protected:
	SceneMgr() {}
	virtual ~SceneMgr() {}

private:
	std::vector<std::shared_ptr<SceneBase> > m_pSceneList;
	std::shared_ptr<ShaderBuffer> m_pShaderBuffer;
	std::shared_ptr<FadeBase> m_pFade;
	Scene_Type::kind m_NowScene;
	Scene_Type::kind m_OldScene;
};