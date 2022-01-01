#include "LV_MasterWitch.h"
#include <App/FactoryMethod.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Transform.h>
#include <App/Component/Mesh.h>
#include <App/Component/Instancing.h>


void LV_MasterWitch::Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)
{
	if (!pObject.expired())
	{
		std::weak_ptr<EventTrigger> pET(pObject.lock()->AddComponent<EventTrigger>());
		std::weak_ptr<Transform> pTransform = pObject.lock()->GetComponent<Transform>();
		if (!pET.expired())
		{
			pET.lock()->SetType(EventTrigger::Type::TALK_1);
		}
		if (!pTransform.expired())
		{
			pTransform.lock()->localpos.z = -30.0f;
		}
	}

	if (pScene.expired())return;

	std::weak_ptr<Event> pTalkEvent;
	std::shared_ptr<Talk> pTalk;

	Object::WORKER_OBJ pMasterWitch = FactoryMethod::GetInstance().CreateBossWitchObject();
	m_pObjectList.emplace_back(pMasterWitch);
	m_pObjectList.emplace_back(FactoryMethod::GetInstance().CreateWater());

	if (!pMasterWitch.expired())
	{
		if (!pMasterWitch.lock()->GetComponent<MasterWitch>().expired())
		{
			pMasterWitch.lock()->GetComponent<MasterWitch>().lock()->SetTarget(pObject);
		}
		pTalkEvent = pMasterWitch.lock()->GetComponent<Event>();

		pTalk = std::move(FactoryMethod::GetInstance().CreateTalkEvent("Assets/csv/lastTalk.csv"));
		pTalk->SetMessageWindow(pMW);
	}

	Object::OWNER_OBJ pTerrain;
	pTerrain.reset(new Object());
	pTerrain->SetType(ObjectType::LEVEL);
	std::weak_ptr<Transform> pTransform = pTerrain->GetComponent<Transform>();
	std::weak_ptr<Renderer3D> pRenderer3D = pTerrain->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pTerrain->AddComponent<Mesh>();
	if (!pMesh.expired() && !pRenderer3D.expired() && !pTransform.expired())
	{
		pTransform.lock()->localscale.x = 50.0f;
		pTransform.lock()->localscale.z = 50.0f;
		pMesh.lock()->Set("field2.obj");
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_TRIPLANAR);
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->SetBumpImage("terrainBump");
	}
	pScene.lock()->AddObject(pTerrain);
	m_pObjectList.emplace_back(pTerrain);

	Object::WORKER_OBJ pOutSideArea;
	std::weak_ptr<Instancing> pInstancing;
	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 25.0f,0.0f,0.0f };
		pTransform.lock()->localscale = { 20.0f,1.0f,30.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(6, 0, 9), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -25.0f,0.0f,0.0f };
		pTransform.lock()->localscale = { 20.0f,1.0f,30.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(6 , 0, 9), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 0.0f,0.0f,25.0f };
		pTransform.lock()->localscale = { 60.0f,1.0f,20.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(18, 0, 6), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 20.0f,0.0f,-25.0f };
		pTransform.lock()->localscale = { 30.0f,1.0f,20.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(9, 0, 6), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -20.0f,0.0f,-25.0f };
		pTransform.lock()->localscale = { 30.0f,1.0f,20.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(9, 0, 6), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pScene.lock()->MoveObject_FactoytoScene();

	if (!pTalkEvent.expired())
	{
		pTalkEvent.lock()->Add<Talk>(pTalk);
		EventMgr::GetInstance().SetEventInfo(pTalkEvent, pScene.lock()->GetObjectList(), EventTrigger::Type::TALK_1);
	}
}

void LV_MasterWitch::Finalize(const Object::WORKER_OBJ pObject)
{
	if (!pObject.expired())
	{
		pObject.lock()->RemoveComponent<EventTrigger>();
	}
	ReleaseObjectList();
}
