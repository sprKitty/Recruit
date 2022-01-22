#include "SceneMgr.h"
#include <App/Scene/TitleScene.h>
#include <App/Scene/GameScene.h>
#include <App/RenderPipeline.h>
#include <App/MeshData.h>
#include <App/TextureData.h>
#include <App/Collision.h>
#include <App/EventMgr.h>
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
	MeshData::GetInstance().Load("grass.obj");
	MeshData::GetInstance().Load("branchField.obj");
	MeshData::GetInstance().Load("fence.obj");
	TextureData::GetInstance().Initialize();
	TextureData::GetInstance().Load("samp.png", "characterchip/");
	TextureData::GetInstance().Load("sampattack.png", "characterchip/");
	TextureData::GetInstance().Load("witch.png", "characterchip/");
	TextureData::GetInstance().Load("hiragana.png");
	TextureData::GetInstance().Load("textframe.png");
	TextureData::GetInstance().Load("terrain.png");
	TextureData::GetInstance().Load("terrainBump.png");
	TextureData::GetInstance().Load("terrainGrass.png");
	TextureData::GetInstance().Load("terrainGrassBump.png");
	TextureData::GetInstance().Load("tree.png");
	TextureData::GetInstance().Load("grass.png");
	TextureData::GetInstance().Load("pushEnter.png");
	TextureData::GetInstance().Load("gameLogo.png");
	TextureData::GetInstance().Load("titleCharacter.png");
	TextureData::GetInstance().Load("noise.png", "GrayScale/");
	TextureData::GetInstance().Load("noiseBump.png", "GrayScale/");
	TextureData::GetInstance().Load("001.png", "GrayScale/");
	m_pShaderBuffer->SetTexturePS(TextureData::GetInstance().Get("noise.png"), ShaderResource::TEX_TYPE::WATER_HEIGHT);
	m_pShaderBuffer->SetTexturePS(TextureData::GetInstance().Get("noiseBump.png"), ShaderResource::TEX_TYPE::WATER_BUMP);
	Collision::GetInstance().Initialize();
	EventMgr::GetInstance().Initialize();
	srand(timeGetTime());

	m_pFade.reset(new Fade());
	m_pFade->Init();

	m_pSceneList.resize(Scene_Type::MAX);
	m_pSceneList[Scene_Type::TITLE].reset(new TitleScene());
	m_pSceneList[Scene_Type::GAME].reset(new GameScene());
	m_NowScene = Scene_Type::TITLE;

	for (const auto& itr : m_pSceneList)
	{
		itr->SetFadeFunction(m_pFade);
		itr->SetShaderBuffer(m_pShaderBuffer);
	}

	m_pSceneList[m_NowScene]->Init();

	//std::unique_ptr<RenderTarget> pRT(new RenderTarget());
	//pRT->Create(256, 256, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	//m_pWaterHeightMap.reset(new Camera());
	//m_pWaterHeightMap->Init();
	//m_pWaterHeightMap->SetRenderTarget(pRT);
	//m_pWaterHeightMap->vpSize.set(256);

	//pRT.reset(new RenderTarget());
	//pRT->Create(256, 256, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	//m_pWaterNormalMap.reset(new Camera());
	//m_pWaterNormalMap->Init();
	//m_pWaterNormalMap->SetRenderTarget(pRT);
	//m_pWaterNormalMap->vpSize.set(256);


	return S_OK;
}

void SceneMgr::Finalize()
{
	m_pFade->Uninit();
	m_pSceneList[m_NowScene]->Uninit();
	EventMgr::GetInstance().Finalize();
	Collision::GetInstance().Finalize();
	TextureData::GetInstance().Finalize();
	MeshData::GetInstance().Finalize();
	RenderPipeline::GetInstance().Finalize();
	m_pShaderBuffer->Finalize();
	UninitInput();
	UninitSound();
	DirectX11::GetInstance().Finalize();

}

void SceneMgr::Update()
{
	m_pShaderBuffer->InitParam();
	m_pShaderBuffer->SetGameStartCnt(Clocker::GetInstance().GetGameTime() * 0.1f);
	UpdateInput();

	if (m_pFade->Update())return;

	m_OldScene = m_NowScene;
	m_NowScene = m_pSceneList[m_NowScene]->Update();
	Collision::GetInstance().MouseColUpdate();
	Collision::GetInstance().Update();
	if (m_OldScene != m_NowScene)
	{
		m_pSceneList[m_OldScene]->Uninit();
		m_pSceneList[m_NowScene]->Init();
		m_pSceneList[m_NowScene]->Update();
		m_pFade->StartFadeOut();
	}
}

void SceneMgr::Draw()
{
	m_pShaderBuffer->SetTexturePS(TextureData::GetInstance().Get("noise.png"), ShaderResource::TEX_TYPE::WATER_HEIGHT);
	m_pShaderBuffer->SetTexturePS(TextureData::GetInstance().Get("noiseBump.png"), ShaderResource::TEX_TYPE::WATER_BUMP);
	m_pSceneList[m_NowScene]->Draw();

	DirectX11::GetInstance().BeginDraw();
	DirectX::XMMATRIX mtx;
	m_pShaderBuffer->BindVS(VS_TYPE::NORMAL);
	m_pShaderBuffer->BindPS(PS_TYPE::NORMAL);
	mtx = MyMath::ConvertMatrix(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0), Vector3(0, 0, 0), Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 10));
	m_pShaderBuffer->SetTexturePS(m_pSceneList[m_NowScene]->GetMainScreenTex());
	m_pShaderBuffer->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();

	m_pFade->Draw(m_pShaderBuffer);
	DirectX11::GetInstance().EndDraw();
}