#include "FactoryMethod.h"
#include <App/Component/Event/EventTrigger.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Mesh.h>
#include <App/Component/Character/Player.h>
#include <App/Component/Character/MasterWitch.h>
#include <App/Component/Transform.h>
#include <App/Component/Collider.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <System/Mouse.h>
#include <App/Event/Talk.h>
#include <App/EventMgr.h>
#include <App/GameKeyBind.h>


Object::WORKER_OBJ FactoryMethod::CreateObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->Init();
	pObj->SetType(ObjectType::NONE);
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("board");
	}
	if (!pBBR.expired())
	{
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
		if (!m_pCamera.expired())
		{
			pBBR.lock()->SetCamera(m_pCamera);
		}
	}
	if (!m_pMouse.expired())
	{
		m_pMouse.lock()->SetExecuteFunc(Delegate<Transform, void, const Vector3&>::CreateDelegator(pObj->GetComponent<Transform>(), &Transform::SetPos));
	}
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB)
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYER);
	pObj->Init();

	std::weak_ptr<Player> pPlayer = pObj->AddComponent<Player>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}
	if (!pPlayer.expired() && !pBBR.expired())
	{
		pPlayer.lock()->SetBillBoardRenderer(pBBR);
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
		pBBR.lock()->SetCamera(m_pCamera);
		//pBBR.lock()->ZaxisUnlock();
		//pBBR.lock()->YaxisLock();
	}
	if (!m_pMouse.expired())
	{
		m_pMouse.lock()->SetExecuteFunc(Delegate<Player, void, const Vector3&>::CreateDelegator(pPlayer, &Player::SetMousePos));
	}
	if (!pGKB.expired())
	{
		pGKB.lock()->SetKeyInfo(KeyBind::MOVE, KeyType::PRESS, VK_RBUTTON, Delegate<Player, void>::CreateDelegator(pPlayer, &Player::EnableChangeDestination), Delegate<Mouse, const bool>::CreateDelegator(m_pMouse, &Mouse::IsNotHitObject));
		pGKB.lock()->SetKeyInfo(KeyBind::ATTACK, KeyType::TRIGGER, VK_RBUTTON, Delegate<Player, void>::CreateDelegator(pPlayer, &Player::EnableAttack), Delegate<Mouse, const bool>::CreateDelegator(m_pMouse, &Mouse::IsHitAnyObject));
	}
	if (!pET.expired())
	{
		pET.lock()->SetType(EventTrigger::Type::TALK_1);
	}
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerMagic()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYERATTACK);
	pObj->Init();

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.5f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("cube");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
	}
	if (!pCollider.expired())
	{
		pCollider.lock()->EnableCollisionType(Collision_Type::BC);
	}
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->Init();
	pObj->SetType(ObjectType::BOSSWITCH);
	pObj->AddComponent<Event>();
	std::weak_ptr<MasterWitch> pMasterWitch = pObj->AddComponent<MasterWitch>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pCollider.expired())
	{
		pCollider.lock()->SetPosDeviation(Vector3(0, 0, 0.9f));
		pCollider.lock()->SetScaleDeviation(0.9f);
		pCollider.lock()->EnableCollisionType(Collision_Type::BC);
		pCollider.lock()->EnableCollisionType(Collision_Type::MOUSE);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}	if (!pMasterWitch.expired() && !pBBR.expired())
	{
		pMasterWitch.lock()->SetBillBoardRenderer(pBBR);
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
		pBBR.lock()->SetCamera(m_pCamera);
		//pBBR.lock()->ZaxisUnlock();
		//pBBR.lock()->YaxisLock();
	}
	std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
	if (!pET.expired())
	{
		pET.lock()->SetType(EventTrigger::Type::TALK_1);
	}
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchMagic()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSATTACK);
	pObj->Init();

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.5f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("cube");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
	}
	if (!pCollider.expired())
	{
		pCollider.lock()->EnableCollisionType(Collision_Type::BC);
	}
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBoss1Object()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->Init();
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateEventObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->Init();
	pObj->AddComponent<Event>();
	m_pObjectList.push_back(pObj);
	return pObj;
}

std::shared_ptr<Talk> FactoryMethod::CreateTalkEvent(const char* pPath)
{
	std::shared_ptr<Talk> pTalk(new Talk());
	pTalk->Load(pPath);
	pTalk->Init();
	return std::move(pTalk);
}