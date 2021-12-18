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
#include <App/Component/Magic/MagicBall.h>
#include <App/Component/Magic/MagicRazer.h>
#include <App/Component/Instancing.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <App/Event/Talk.h>
#include <App/EventMgr.h>
#include <App/GameKeyBind.h>
#include <System/Mouse.h>


Object::WORKER_OBJ FactoryMethod::CreateObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::NONE);
	m_pObjectList.push_back(pObj);

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
			pBBR.lock()->ZaxisUnlock();
			pBBR.lock()->XaxisLock();
			pBBR.lock()->SetCamera(m_pCamera);
		}
	}
	if (!m_pMouse.expired())
	{
		m_pMouse.lock()->SetExecuteFunc(Delegate<Transform, void, const Vector3&>::CreateDelegator(pObj->GetComponent<Transform>(), &Transform::SetPos));
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateTree()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::OUTSIDE);
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();

	if (!pTransform.expired())
	{
		pTransform.lock()->localrot.y = 20;
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("tree");
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("tree2");
	}

	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateOutsideArea()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::OUTSIDE);
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Instancing> pInst = pObj->AddComponent<Instancing>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pCollider.expired())
	{
		pCollider.lock()->EnableMouseCollision();
		pCollider.lock()->SetCollisionType(ObjectType::PLAYERATTACK, CollisionType::AABB);
		pCollider.lock()->SetCollisionType(ObjectType::BOSSATTACK1, CollisionType::AABB);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("cube");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->SetBumpImage("terrainBump");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_TRIPLANAR);
	}
	if (!pInst.expired())
	{
		pInst.lock()->Set("tree2");
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB)
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYER);
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Player> pPlayer = pObj->AddComponent<Player>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pCollider.expired())
	{
		pCollider.lock()->SetScaleDeviation(0.5f);
		pCollider.lock()->SetPosDeviation(Vector3(0.0f, 0.0f , 0.0f));
		pCollider.lock()->SetCollisionType(ObjectType::BOSSATTACK1, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::BOSSATTACK2, CollisionType::OBB);
		pCollider.lock()->SetCollisionType(ObjectType::BOSSWITCH, CollisionType::BC);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}
	if (!pPlayer.expired() && !pBBR.expired())
	{
		pPlayer.lock()->SetBillBoardRenderer(pBBR);
		pBBR.lock()->ZaxisUnlock();
		pBBR.lock()->XaxisLock();
		pBBR.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
		pBBR.lock()->SetCamera(m_pCamera);
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
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerMagic()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYERATTACK);
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.2f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("sphere");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
	}
	if (!pCollider.expired())
	{
		pCollider.lock()->SetCollisionType(ObjectType::BOSSWITCH, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE, CollisionType::AABB);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSWITCH);
	m_pObjectList.push_back(pObj);

	pObj->AddComponent<Event>();
	std::weak_ptr<MasterWitch> pMasterWitch = pObj->AddComponent<MasterWitch>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pCollider.expired())
	{
		pCollider.lock()->SetScaleDeviation(0.5f);
		pCollider.lock()->SetPosDeviation(Vector3(0, 0, 0.0f));
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::PLAYERATTACK, CollisionType::BC);
		pCollider.lock()->EnableMouseCollision();
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}	
	if (!pMasterWitch.expired() && !pBBR.expired())
	{
		pMasterWitch.lock()->SetBillBoardRenderer(pBBR);
		pBBR.lock()->ZaxisUnlock();
		pBBR.lock()->XaxisLock();	
		pBBR.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
		pBBR.lock()->SetCamera(m_pCamera);
	}
	std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
	if (!pET.expired())
	{
		pET.lock()->SetType(EventTrigger::Type::TALK_1);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchMagicBullet()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSATTACK1);
	pObj->DisableActive();
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.2f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("sphere");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
	}
	if (!pCollider.expired())
	{
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE, CollisionType::AABB);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchMagicBall()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSATTACK2);
	pObj->DisableActive();
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBall> pMB = pObj->AddComponent<MagicBall>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.3f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("sphere");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
	}
	if (!pCollider.expired())
	{
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::BC);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchRazer()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSATTACK2);
	pObj->DisableActive();
	m_pObjectList.push_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicRazer> pMR = pObj->AddComponent<MagicRazer>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pCollider.expired())
	{
		pCollider.lock()->SetScaleDeviation(Vector3(0.5f, 0.5f, 1.0f));
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::OBB);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("cube");
	}	
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBoss1Object()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.push_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateEventObject()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.push_back(pObj);
	pObj->AddComponent<Event>();
	return pObj;
}

std::shared_ptr<Talk> FactoryMethod::CreateTalkEvent(const char* pPath)
{
	std::shared_ptr<Talk> pTalk(new Talk());
	pTalk->Load(pPath);
	pTalk->Init();
	return std::move(pTalk);
}

void FactoryMethod::MoveObjectList(Object::OWNER_OBJECTLIST & pObjectList)
{
	for (int i = 0; i < m_pObjectList.size(); ++i)
	{
		std::weak_ptr<Transform> pT = m_pObjectList[i]->GetComponent<Transform>();
		if (!pT.expired())
		{
			m_pObjectList[i]->EndSortComponent(pT);
		}
		pObjectList.push_back(m_pObjectList[i]);
	}
	m_pObjectList.clear();
}
