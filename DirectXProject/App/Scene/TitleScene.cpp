#include "TitleScene.h"
#include <App/RenderPipeline.h>
#include <App/FactoryMethod.h>
#include <App/Level/LV_MasterWitch.h>
#include <App/Level/LV_StageBranch.h>
#include <App/ViewPoint/RotationCamera.h>
#include <App/Collision.h>
#include <System/RenderTarget.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>

void TitleScene::Init()
{
	m_isNextScene = false;

	std::unique_ptr<RenderTarget> pRT(new RenderTarget());
	pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	pRT->Addition(DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_pCamera.reset(new RotationCamera());
	m_pCamera->Init(); 
	m_pCamera->look.set(0);
	m_pCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	pRT->CreateDepthStencil(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pReflectionCamera.reset(new Camera());
	m_pReflectionCamera->Init();
	m_pReflectionCamera->vpSize.set({ SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f });
	m_pReflectionCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	pRT->Addition(DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_pBlurXCamera.reset(new Camera());
	m_pBlurXCamera->Init();
	m_pBlurXCamera->vpSize.set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT });
	m_pBlurXCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	pRT->Addition(DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_pBlurYCamera.reset(new Camera());
	m_pBlurYCamera->Init();
	m_pBlurYCamera->vpSize.set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f });
	m_pBlurYCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(2048, 2048, DXGI_FORMAT_R32G32B32A32_FLOAT, 1);
	pRT->CreateDepthStencil(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pLight.reset(new Light());
	m_pLight->Init();
	m_pLight->SetRenderTarget(pRT);
	m_pLight->parallelScale.set(400);
	m_pLight->position.set(Vector3(3, 4, 4));
	m_pLight->fov.set(60.0f);
	m_pLight->color.set(Vector4(255.0f / 255.0f, 161.0f / 255.0f, 3.0f / 255.0f, 1.0f));
	m_pLight->perspective.set(false);
	m_pLight->jaggiesReduction.set(0.00005f);

	m_pMouse.reset(new Mouse());
	m_pMouse->Initialize();

	FactoryMethod::GetInstance().SetCamera(m_pCamera);
	FactoryMethod::GetInstance().SetMouse(m_pMouse);
	m_pMouse->SetCamera(m_pCamera);

	Object::WORKER_OBJ pCharacter = FactoryMethod::GetInstance().CreateObject();
	std::weak_ptr<Transform> pTransform = pCharacter.lock()->GetComponent<Transform>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.3f;
		pTransform.lock()->localpos = { 0.0f,0.13f,0.35f };
	}

	Object::WORKER_OBJ pGameLogo = FactoryMethod::GetInstance().CreateObject();
	pTransform = pGameLogo.lock()->GetComponent<Transform>();
	std::weak_ptr<Renderer3D> pR3D = pGameLogo.lock()->GetComponent<Renderer3D>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.3f;
		pTransform.lock()->localrot.y = 90.f;
		pTransform.lock()->localpos = { 0.35f,0.13f,0.0f };
	}
	if (!pR3D.expired())
	{
		pR3D.lock()->SetMainImage("gameLogo");
	}

	Object::WORKER_OBJ pFence = FactoryMethod::GetInstance().CreateFence();

	Object::WORKER_OBJ pTerrain = FactoryMethod::GetInstance().CreateTerrain();
	pTransform = pTerrain.lock()->GetComponent<Transform>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = { 10.0f,1.0f,10.0f };
		pTransform.lock()->localpos.y = 0.0f;
	}

	Object::WORKER_OBJ pWater = FactoryMethod::GetInstance().CreateWater();
	pTransform = pWater.lock()->GetComponent<Transform>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = { 0.25f,1.0f,0.25f };
		pTransform.lock()->localpos.y = 0.01f;
	}
}

void TitleScene::Uninit()
{
	m_pObjList.clear();
	m_pMouse->Finalize();
}

const Scene_Type::kind TitleScene::Update()
{
	Scene_Type::kind scene = Scene_Type::TITLE;

	MoveObject_FactoytoScene();
	m_pMouse->SetScreenPos(GetMousePosX(), GetMousePosY());
	m_pMouse->CalcScreentoXZ();

	DeleteObject();
	EventMgr::GetInstance().Update();
	for (auto pObj : m_pObjList)
	{
		if (pObj->IsActive())
		{
			pObj->Update();
		}
	}

	m_pCamera->Update();
	m_pLight->Update();

	if (m_isNextScene)
	{
		return Scene_Type::GAME;
	}

	if (IsKeyTrigger(VK_RETURN))
	{
		m_pFadeInFunction->Execute();
		m_isNextScene = true;
	}

	return scene;
}

void TitleScene::Draw()
{
	RenderPipeline::GetInstance().DrawShadowMap(m_pLight);

	// …–Ê”½ŽË
	m_pCamera->CalcReflect();
	m_pReflectionCamera->CopyParameter(m_pCamera);
	RenderPipeline::GetInstance().DrawWaterReflection(m_pReflectionCamera, m_pLight, m_pLight);
	m_pCamera->Restore();

	// ƒƒCƒ“‰æ–Ê
	RenderPipeline::GetInstance().Draw(m_pCamera, m_pLight, m_pLight, m_pReflectionCamera);

	m_pMainScreen->BindRenderTarget();
	m_pMainScreen->Bind2D(m_pShaderBuffer);

	DirectX::XMMATRIX mtx;
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::NORMAL);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::NORMAL);
	m_pShaderBuffer.lock()->SetTexturePS(m_pCamera->GetRenderingTexture(0));
	mtx = MyMath::ConvertMatrix(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0), Vector3(0, 0, 0), Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 10));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}
