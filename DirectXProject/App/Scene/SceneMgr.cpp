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
#include <System/Clocker.h>
#include <random>
#include <time.h>

HRESULT SceneMgr::Init(HWND hWnd, UINT width, UINT height)
{
	// ƒVƒXƒeƒ€‚Ì‰Šú‰»
	DirectX11::GetInstance().Start(hWnd, width, height, false);
	InitSound();
	Geometory::GetInstance().Create();
	InitInput();
	m_pShaderBuffer.reset(new ShaderBuffer());
	m_pShaderBuffer->Initialize();
	RenderPipeline::GetInstance().Initialize();
	RenderPipeline::GetInstance().SetShaderBuffer(m_pShaderBuffer);
	MessageWindow::CreateOffsetMap();
	MeshData::GetInstance().Initialize();
	MeshData::GetInstance().Load("field2.obj");
	MeshData::GetInstance().Load("sphere.obj");
	MeshData::GetInstance().Load("tree2.obj");
	TextureData::GetInstance().Initialize();
	TextureData::GetInstance().Load("samp.png", "characterchip/");
	TextureData::GetInstance().Load("sampattack.png", "characterchip/");
	TextureData::GetInstance().Load("witch.png", "characterchip/");
	TextureData::GetInstance().Load("hiragana.png");
	TextureData::GetInstance().Load("textframe.png");
	TextureData::GetInstance().Load("terrain.png");
	TextureData::GetInstance().Load("terrainBump.png");
	TextureData::GetInstance().Load("tree.png");
	TextureData::GetInstance().Load("noise.png", "GrayScale/");
	TextureData::GetInstance().Load("noiseBump.png", "GrayScale/");
	m_pShaderBuffer->SetTexture(TextureData::GetInstance().Get("noise.png"), ShaderResource::TEX_TYPE::WATER_HEIGHT);
	m_pShaderBuffer->SetTexture(TextureData::GetInstance().Get("noiseBump.png"), ShaderResource::TEX_TYPE::WATER_BUMP);
	Collision::GetInstance().Initialize();
	srand(timeGetTime());

	m_NowScene = Scene_Type::SCENE_GAME;
	m_pSceneList.resize(Scene_Type::SCENE_MAX);
	m_pSceneList[m_NowScene].reset(new Game());
	m_pSceneList[m_NowScene]->Init();
	m_pSceneList[m_NowScene]->SetShaderBuffer(m_pShaderBuffer);
	return S_OK;
}

void SceneMgr::Finalize()
{
	m_pSceneList[m_NowScene]->Uninit();
	Collision::GetInstance().Finalize();
	TextureData::GetInstance().Finalize();
	MeshData::GetInstance().Finalize();
	RenderPipeline::GetInstance().Finalize();
	m_pShaderBuffer->Finalize();
	UninitInput();
	UninitSound();
	DirectX11::GetInstance().Finalize();

}

float g_op = 0;
void SceneMgr::Update()
{
	m_pShaderBuffer->InitParam();
	g_op += 0.005f;
	m_pShaderBuffer->SetGameStartCnt(g_op);
	UpdateInput();
	m_pSceneList[m_NowScene]->Update();
	m_pSceneList[m_NowScene]->DeleteObject();
	Collision::GetInstance().Update();
}

void SceneMgr::Draw()
{
	m_pShaderBuffer->SetTexture(TextureData::GetInstance().Get("noise.png"), ShaderResource::TEX_TYPE::WATER_HEIGHT);
	m_pShaderBuffer->SetTexture(TextureData::GetInstance().Get("noiseBump.png"), ShaderResource::TEX_TYPE::WATER_BUMP);
	m_pSceneList[m_NowScene]->Draw();
}