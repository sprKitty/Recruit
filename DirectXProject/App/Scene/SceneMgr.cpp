#include "SceneMgr.h"
#include <App/RenderPipeline.h>
#include <App/Scene/Game.h>
#include <App/MeshData.h>
#include <App/TextureData.h>
#include <App/Collision.h>
#include <System/MessageWindow.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>
#include <System/Input.h>
#include <random>
#include <time.h>

HRESULT SceneMgr::Init(HWND hWnd, UINT width, UINT height)
{
	// ƒVƒXƒeƒ€‚Ì‰Šú‰»
	DirectX11::GetInstance().Start(hWnd, width, height, false);
	InitSound();
	Geometory::GetInstance().Create();
	InitInput();
	ShaderBuffer::GetInstance().Initialize();
	RenderPipeline::GetInstance().Initialize();
	MessageWindow::CreateOffsetMap();
	MeshData::GetInstance().Initialize();
	MeshData::GetInstance().Load("field2.obj");
	MeshData::GetInstance().Load("sphere.obj");
	TextureData::GetInstance().Initialize();
	TextureData::GetInstance().Load("samp.png", "characterchip/");
	TextureData::GetInstance().Load("sampattack.png", "characterchip/");
	TextureData::GetInstance().Load("witch.png", "characterchip/");
	TextureData::GetInstance().Load("hiragana.png");
	TextureData::GetInstance().Load("textframe.png");
	TextureData::GetInstance().Load("terrain.png");
	TextureData::GetInstance().Load("terrainBump.png");
	Collision::GetInstance().Initialize();
	srand(timeGetTime());

	m_NowScene = Scene_Type::SCENE_GAME;
	m_pSceneList.resize(Scene_Type::SCENE_MAX);
	m_pSceneList[m_NowScene].reset(new Game());
	m_pSceneList[m_NowScene]->Init();

	return S_OK;
}

void SceneMgr::Finalize()
{
	m_pSceneList[m_NowScene]->Uninit();
	Collision::GetInstance().Finalize();
	TextureData::GetInstance().Finalize();
	MeshData::GetInstance().Finalize();
	RenderPipeline::GetInstance().Finalize();
	ShaderBuffer::GetInstance().Finalize();
	UninitInput();
	UninitSound();
	DirectX11::GetInstance().Finalize();

}

void SceneMgr::Update()
{
	ShaderBuffer::GetInstance().InitParam();
	UpdateInput();
	m_pSceneList[m_NowScene]->Update();
	m_pSceneList[m_NowScene]->DeleteObject();
	Collision::GetInstance().Update();
}

void SceneMgr::Draw()
{
	m_pSceneList[m_NowScene]->Draw();
}