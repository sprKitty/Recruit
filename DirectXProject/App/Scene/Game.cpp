#include <App/Scene/Game.h>
#include <App/RenderPipeline.h>
#include <App/FactoryMethod.h>
#include <App/Level.h>
#include <System/MessageWindow.h>
#include <System/Geometory.h>
#include <System/Sound/Sound.h>

void Game::Init()
{
	m_pCamera.reset(new Camera());
	m_pCamera->Init(false);

	m_pMessageWindow.reset(new MessageWindow());
	m_pMessageWindow->Initialize();
	
	m_pMouse.reset(new Mouse());
	m_pMouse->Initialize();

	Level level;
	level.Create(weak_from_this());

	FactoryMethod::GetInstance().SetCamera(m_pCamera);
	FactoryMethod::GetInstance().SetMouse(m_pMouse);

	Object::OWNER_OBJ pObj = FactoryMethod::GetInstance().CreateObject();

	Object::OWNER_OBJ pPlayer = FactoryMethod::GetInstance().CreatePlayerObject();
	
	Object::OWNER_OBJ pBoss1 = FactoryMethod::GetInstance().CreateBoss1Object();

	Object::OWNER_OBJ pMasterWitch = FactoryMethod::GetInstance().CreateBossWitchObject();

	std::shared_ptr<Talk> pTalk = std::move(FactoryMethod::GetInstance().CreateTalkEvent("Assets/csv/lastTalk.csv"));
	pTalk->SetMessageWindow(m_pMessageWindow);

	Object::OWNER_OBJ pEvent = FactoryMethod::GetInstance().CreateEventObject();
	std::weak_ptr<Event> pTalkEvent = pEvent->GetComponent<Event>();
	
	if (!pMasterWitch->GetComponent<MasterWitch>().expired())
	{
		pMasterWitch->GetComponent<MasterWitch>().lock()->SetTarget(pPlayer);
	}

	m_pObjList.push_back(pObj);
	m_pObjList.push_back(pPlayer);
	m_pObjList.push_back(pBoss1);
	m_pObjList.push_back(pMasterWitch);
	m_pObjList.push_back(pEvent);

	Object::WORKER_OBJECTLIST pObjects = Object::ConvertWorker(m_pObjList);
	if (!pTalkEvent.expired())
	{
		pTalkEvent.lock()->Add<Talk>(pTalk);
		EventMgr::GetInstance().SetEventInfo(pTalkEvent, pObjects, EventTrigger::Type::TALK_1);
	}

	RenderPipeline::GetInstance().SetCamera(m_pCamera);

}

void Game::Uninit()
{
	m_pObjList.clear();
	m_pMouse->Finalize();
	m_pMessageWindow->Finalize();
}

Scene_Type::kind Game::Update()
{
	Scene_Type::kind scene = Scene_Type::SCENE_GAME;
	m_pMouse->SetScreenPos(GetMousePosX(), GetMousePosY());
	m_pMouse->CalcScreentoXZ(m_pCamera->GetView(), m_pCamera->GetProj());
	
	EventMgr::GetInstance().CallFunc();
	for (auto pObj : m_pObjList)
	{
		pObj->Update();
	}

	m_pCamera->Update();

	return scene;
}

void Game::Draw()
{
	BeginDrawDX();

	RenderPipeline::GetInstance().Draw(DrawType::WORLD_OF_NORMAL);

	m_pCamera->Bind2D();
	ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
	ShaderBuffer::GetInstance().BindPS(PS_TYPE::NORMAL);
	m_pMessageWindow->Draw();

	EndDrawDX();
}