#include <App/Scene/Game.h>
#include <App/RenderPipeline.h>
#include <App/FactoryMethod.h>
#include <App/Level.h>
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
#include <System/MessageWindow.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>
#include <System/Mouse.h>


void Game::Init()
{
	m_pCamera.reset(new Camera());
	m_pCamera->Init();
	//m_pCamera->EnableLate();

	m_pLight.reset(new Light());
	m_pLight->Init();

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

	Object::WORKER_OBJ pPlayer = FactoryMethod::GetInstance().CreatePlayerObject(m_pKeyBind);
	if (!pPlayer.expired())
	{
		m_pCamera->SetTarget(pPlayer.lock()->GetComponent<Transform>());
	}

	Object::WORKER_OBJ pBoss1 = FactoryMethod::GetInstance().CreateBoss1Object();

	Object::WORKER_OBJ pMasterWitch = FactoryMethod::GetInstance().CreateBossWitchObject();
	std::weak_ptr<Event> pTalkEvent = pMasterWitch.lock()->GetComponent<Event>();

	std::shared_ptr<Talk> pTalk = std::move(FactoryMethod::GetInstance().CreateTalkEvent("Assets/csv/lastTalk.csv"));
	pTalk->SetMessageWindow(m_pMessageWindow);

	
	if (!pMasterWitch.lock()->GetComponent<MasterWitch>().expired())
	{
		pMasterWitch.lock()->GetComponent<MasterWitch>().lock()->SetTarget(pPlayer);
	}

	Object::WORKER_OBJECTLIST pObjects = Object::ConvertWorker(m_pObjList);
	if (!pTalkEvent.expired())
	{
		pTalkEvent.lock()->Add<Talk>(pTalk);
		EventMgr::GetInstance().SetEventInfo(pTalkEvent, pObjects, EventTrigger::Type::TALK_1);
	}

	Level level;
	level.Create(weak_from_this());

	RenderPipeline::GetInstance().SetCamera(m_pCamera);
	RenderPipeline::GetInstance().SetLight(m_pLight);
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
	FactoryMethod::GetInstance().MoveObjectList(m_pObjList);
	m_pKeyBind->Update();
	m_pMouse->SetScreenPos(GetMousePosX(), GetMousePosY());
	m_pMouse->CalcScreentoXZ();

	EventMgr::GetInstance().CallFunc();
	for (auto pObj : m_pObjList)
	{
		if (pObj->IsActive())
		{
			pObj->Update();
		}
	}

	m_pCamera->Update();
	m_pLight->Update();
	return scene;
}

void Game::Draw()
{
	RenderPipeline::GetInstance().Write(WriteType::DEPTH_OF_SHADOW);
	
	DirectX11::GetInstance().BeginDraw();
	RenderPipeline::GetInstance().Draw(DrawType::WORLD_OF_NORMAL);
	RenderPipeline::GetInstance().Draw(DrawType::WORLD_OF_EFFECT);
	RenderPipeline::GetInstance().Draw(DrawType::WORLD_OF_TRIPLANAR);
	RenderPipeline::GetInstance().Draw(DrawType::WORLD_OF_CHARACTER);
	Collision::GetInstance().Draw();

	m_pCamera->Bind2D();
	ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
	ShaderBuffer::GetInstance().BindPS(PS_TYPE::CHARACTER);
	m_pMessageWindow->Draw();

	
	ShaderBuffer::GetInstance().BindPS(PS_TYPE::NORMAL);
	ShaderBuffer::GetInstance().SetTexture(RenderPipeline::GetInstance().GetRenderTex(WriteType::DEPTH_OF_SHADOW));
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(200, 200, 0), Vector3(0, 0, 0), Vector3(100, 100, 0));
	ShaderBuffer::GetInstance().SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
	DirectX11::GetInstance().EndDraw();
}