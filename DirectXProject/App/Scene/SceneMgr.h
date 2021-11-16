#pragma once
#include <System/ClassDesign/Singleton.h>
#include <System/DirectX.h>
#include <App/Scene/SceneBase.h>

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
	Scene_Type::kind m_NowScene;
	Scene_Type::kind m_OldScene;
};