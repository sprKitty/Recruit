#include "SceneMgr.h"
#include <App/RenderPipeline.h>
#include <App/Scene/Game.h>
#include <System/MessageWindow.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>
#include <System/Input.h>
#include <random>
#include <time.h>

HRESULT SceneMgr::Init(HWND hWnd, UINT width, UINT height)
{
	// ƒVƒXƒeƒ€‚Ì‰Šú‰»
	InitDX(hWnd, width, height, false);
	InitSound();
	Geometory::GetInstance().Create();
	InitInput();
	ShaderBuffer::GetInstance().Initalize();
	RenderPipeline::GetInstance().Initialize();
	MessageWindow::CreateOffsetMap();
	srand(timeGetTime());

	m_NowScene = Scene_Type::SCENE_GAME;
	m_pSceneList.resize(Scene_Type::SCENE_MAX);
	m_pSceneList[m_NowScene].reset(new Game());
	m_pSceneList[m_NowScene]->Init();

	return S_OK;
}

void SceneMgr::Uninit()
{
	m_pSceneList[m_NowScene]->Uninit();
	RenderPipeline::GetInstance().Finalize();
	ShaderBuffer::GetInstance().Finalize();
	UninitInput();
	UninitSound();
	UninitDX();

}

void SceneMgr::Update()
{
	ShaderBuffer::GetInstance().InitParam();
	UpdateInput();
	m_pSceneList[m_NowScene]->Update();
}

void SceneMgr::Draw()
{
	m_pSceneList[m_NowScene]->Draw();
}