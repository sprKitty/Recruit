#include <App/Scene/Game.h>
#include <App/RenderPipeline.h>
#include <App/FactoryMethod.h>
#include <App/Level/LV_MasterWitch.h>
#include <App/Level/LV_StageBranch.h>
#include <App/Collision.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <App/GameKeyBind.h>
#include <App/Component/Transform.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Character/MasterWitch.h>
#include <App/Component/Event/EventTrigger.h>
#include <App/Event/Talk.h>
#include <App/EventMgr.h>
#include <System/RenderTarget.h>
#include <System/MessageWindow.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>


void Game::Init()
{
	std::unique_ptr<RenderTarget> pRT(new RenderTarget());
	pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	pRT->Addition(DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_pCamera.reset(new Camera());
	m_pCamera->Init();
	m_pCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	pRT->CreateDepthStencil(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pReflectionCamera.reset(new Camera());
	m_pReflectionCamera->Init();
	m_pReflectionCamera->vpSize.set({ SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f });
	m_pReflectionCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	m_pCameraDepth.reset(new Camera());
	m_pCameraDepth->Init();
	m_pCameraDepth->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pKawaseBloom.reset(new Camera());
	m_pKawaseBloom->Init();
	m_pKawaseBloom->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurXCamera.reset(new Camera());
	m_pBlurXCamera->Init();
	m_pBlurXCamera->vpSize.set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT });
	m_pBlurXCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurYCamera.reset(new Camera());
	m_pBlurYCamera->Init();
	m_pBlurYCamera->vpSize.set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f });
	m_pBlurYCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurXCameraHalf.reset(new Camera());
	m_pBlurXCameraHalf->Init();
	m_pBlurXCameraHalf->vpSize.set({ SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f });
	m_pBlurXCameraHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurYCameraHalf.reset(new Camera());
	m_pBlurYCameraHalf->Init();
	m_pBlurYCameraHalf->vpSize.set({ SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f });
	m_pBlurYCameraHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.25f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurXCameraHalfHalf.reset(new Camera());
	m_pBlurXCameraHalfHalf->Init();
	m_pBlurXCameraHalfHalf->vpSize.set({ SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.25f });
	m_pBlurXCameraHalfHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.125f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurYCameraHalfHalf.reset(new Camera());
	m_pBlurYCameraHalfHalf->Init();
	m_pBlurYCameraHalfHalf->vpSize.set({ SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.125f });
	m_pBlurYCameraHalfHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.0625f, SCREEN_HEIGHT * 0.125f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurXCameraHalfHalfHalf.reset(new Camera());
	m_pBlurXCameraHalfHalfHalf->Init();
	m_pBlurXCameraHalfHalfHalf->vpSize.set({ SCREEN_WIDTH * 0.0625f, SCREEN_HEIGHT * 0.125f });
	m_pBlurXCameraHalfHalfHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH * 0.0625f, SCREEN_HEIGHT * 0.0625f, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pBlurYCameraHalfHalfHalf.reset(new Camera());
	m_pBlurYCameraHalfHalfHalf->Init();
	m_pBlurYCameraHalfHalfHalf->vpSize.set({ SCREEN_WIDTH * 0.0625f, SCREEN_HEIGHT * 0.0625f });
	m_pBlurYCameraHalfHalfHalf->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, Vector4(0.f, 0.f, 0.f, 0.f));
	m_pEffectCamera.reset(new Camera());
	m_pEffectCamera->Init();
	m_pEffectCamera->vpSize.set({ SCREEN_WIDTH, SCREEN_HEIGHT });
	m_pEffectCamera->SetRenderTarget(pRT);

	pRT.reset(new RenderTarget());
	pRT->Create(2048, 2048, DXGI_FORMAT_R32G32B32A32_FLOAT, 1);
	pRT->CreateDepthStencil(DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pLight.reset(new Light());
	m_pLight->Init();
	m_pLight->SetRenderTarget(pRT);
	m_pLight->parallelScale.set(16);
	m_pLight->position.set(Vector3(40, 50, -100));
	m_pLight->fov.set(60.0f);
	m_pLight->color.set(Vector4(255.0f / 255.0f, 161.0f / 255.0f, 3.0f / 255.0f, 1.0f));
	m_pLight->perspective.set(false);

	m_pMessageWindow.reset(new MessageWindow());
	m_pMessageWindow->Initialize();
	
	m_pMouse.reset(new Mouse());
	m_pMouse->Initialize();

	m_pKeyBind.reset(new GameKeyBind());
	m_pKeyBind->Initialize();

	FactoryMethod::GetInstance().SetCamera(m_pCamera);
	FactoryMethod::GetInstance().SetMouse(m_pMouse);
	Collision::GetInstance().SetMouse(m_pMouse);
	m_pMouse->SetCamera(m_pCamera);

	Object::WORKER_OBJ pObj = FactoryMethod::GetInstance().CreateObject();

	m_pPlayer = FactoryMethod::GetInstance().CreatePlayerObject(m_pKeyBind);

	if (!m_pPlayer.expired())
	{
		m_pCamera->targetTransform.set(m_pPlayer.lock()->GetComponent<Transform>());
	}

	m_pLevel.reset(new LV_StageBranch());
	m_pLevel->Initialize(weak_from_this(), m_pPlayer, m_pMessageWindow);
}

void Game::Uninit()
{
	m_pObjList.clear();
	m_pKeyBind->Finalize();
	m_pMouse->Finalize();
	m_pMessageWindow->Finalize();
}

Scene_Type::kind Game::Update()
{
	Scene_Type::kind scene = Scene_Type::SCENE_GAME;
	MoveObject_FactoytoScene();
	m_pKeyBind->Update();
	m_pMouse->SetScreenPos(GetMousePosX(), GetMousePosY());
	m_pMouse->CalcScreentoXZ();


	EventMgr::GetInstance().CallFunc();

	switch (m_pLevel->Transition(m_pPlayer))
	{
	case Level_Type::BRANCH:
		m_pLevel.reset(new LV_StageBranch());
		m_pLevel->Initialize(weak_from_this(), m_pPlayer, m_pMessageWindow);
		break;
	case Level_Type::MASTERWITCH:
		m_pLevel.reset(new LV_MasterWitch());
		m_pLevel->Initialize(weak_from_this(), m_pPlayer, m_pMessageWindow);
		break;
	case Level_Type::DRAGON1:
		break;
	case Level_Type::DRAGON2:
		break;
	case Level_Type::TUTORIAL:
		break;
	case Level_Type::MAX:
	default:
		for (auto pObj : m_pObjList)
		{
			if (pObj->IsActive())
			{
				pObj->Update();
			}
		}
		break;
	}

	m_pCamera->Update();
	m_pLight->Update();
	m_pCameraDepth->CopyParameter(m_pCamera);
	m_pEffectCamera->CopyParameter(m_pCamera);

	return scene;
}

void Game::Draw()
{
	RenderPipeline::GetInstance().DrawShadowMap(m_pLight);
	
	// …–Ê”½ŽË
	m_pCamera->CalcRefletion();
	m_pReflectionCamera->CopyParameter(m_pCamera);
	RenderPipeline::GetInstance().DrawWaterReflection(m_pReflectionCamera, m_pLight, m_pLight);
	m_pCamera->Restore();

	// ƒƒCƒ“‰æ–Ê
	RenderPipeline::GetInstance().Draw(m_pCamera, m_pLight, m_pLight, m_pReflectionCamera);
	RenderPipeline::GetInstance().PlayGaussianBlurX(m_pBlurXCamera, m_pCamera);
	RenderPipeline::GetInstance().PlayGaussianBlurY(m_pBlurYCamera, m_pBlurXCamera);
	RenderPipeline::GetInstance().DrawDOFTexture(m_pCameraDepth, m_pCamera, m_pCamera, m_pBlurYCamera);

	// ƒuƒ‹[ƒ€ˆ—
	RenderPipeline::GetInstance().DrawEffect(m_pEffectCamera);
	RenderPipeline::GetInstance().PlayGaussianBlurX(m_pBlurXCamera, m_pEffectCamera, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurY(m_pBlurYCamera, m_pBlurXCamera, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurX(m_pBlurXCameraHalf, m_pBlurYCamera, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurY(m_pBlurYCameraHalf, m_pBlurXCameraHalf, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurX(m_pBlurXCameraHalfHalf, m_pBlurYCameraHalf, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurY(m_pBlurYCameraHalfHalf, m_pBlurXCameraHalfHalf, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurX(m_pBlurXCameraHalfHalfHalf, m_pEffectCamera, 2);
	RenderPipeline::GetInstance().PlayGaussianBlurY(m_pBlurYCameraHalfHalfHalf, m_pBlurXCameraHalfHalfHalf, 2);
	RenderPipeline::KAWASE_BlOOM_VP pVPList =
	{
		m_pEffectCamera,
		m_pBlurYCamera,
		m_pBlurYCameraHalf,
		m_pBlurYCameraHalfHalf,
		m_pBlurYCameraHalfHalfHalf,
	};
	RenderPipeline::GetInstance().PlayKawaseBloom(m_pKawaseBloom, pVPList);

	DirectX11::GetInstance().BeginDraw();

	m_pCamera->Bind2D(m_pShaderBuffer);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::NORMAL);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::CHARACTER);
	m_pMessageWindow->Draw(m_pShaderBuffer);

	m_pShaderBuffer.lock()->BindPS(PS_TYPE::NORMAL);
	m_pShaderBuffer.lock()->SetTexturePS(m_pKawaseBloom->GetRenderingTexture(0));
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.2f, 0), Vector3(0, 0, 0), Vector3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();

	m_pShaderBuffer.lock()->SetTexturePS(m_pKawaseBloom->GetRenderingTexture(0), ShaderResource::TEX_TYPE::EFFECT);
	m_pShaderBuffer.lock()->SetTexturePS(m_pCameraDepth->GetRenderingTexture(0));
	mtx = MyMath::ConvertMatrix(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0), Vector3(0, 0, 0), Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 10));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();


	DirectX11::GetInstance().EndDraw();
}