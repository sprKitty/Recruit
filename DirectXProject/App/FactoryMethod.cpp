#include "FactoryMethod.h"
#include <App/RenderPipeline.h>

Object::WORKER_OBJ FactoryMethod::CreateObject()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::NONE);
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Renderer3D> pR3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}
	if (!pR3D.expired())
	{
		if (!m_pCamera.expired())
		{
			pR3D.lock()->SetMainImage("titleCharacter");
			pR3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
			pR3D.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
		}
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateWater()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();

	if (!pTransform.expired())
	{
		pTransform.lock()->localpos.y = 0.05f;
		pTransform.lock()->localscale.x = 100.0f;
		pTransform.lock()->localscale.z = 100.0f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("field2");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_WATER);
	}

	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateTerrain()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();

	if (!pTransform.expired())
	{
		pTransform.lock()->localpos.y = 20.0f;
		pTransform.lock()->localscale.x = 20.0f;
		pTransform.lock()->localscale.z = 20.0f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("field2");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->SetBumpImage("terrainBump");
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_TRIPLANAR);
	}

	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateOutsideArea()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::OUTSIDE);
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Instancing> pInst = pObj->AddComponent<Instancing>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pCollider.expired())
	{
		pCollider.lock()->EnableMouseCollision();
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::AABB);
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
		pInst.lock()->Set("tree");
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateFence()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::OUTSIDE);
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("fence");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("fence");
		pRenderer3D.lock()->SetBumpImage("fenceBump");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_TRIPLANAR);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateTransitionLevel()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Instancing> pInst = pObj->AddComponent<Instancing>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	if (!pCollider.expired())
	{
		pCollider.lock()->DisableMouseCollision();
		pCollider.lock()->SetCollisionType(ObjectType::PLAYER, CollisionType::AABB);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("cube");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->SetMainImage("grass");
		pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
	}
	if (!pInst.expired())
	{
		pInst.lock()->Set("grass");
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB)
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYER);
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Player> pPlayer = pObj->AddComponent<Player>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pCollider.expired())
	{
		pCollider.lock()->SetScaleDeviation(0.5f);
		pCollider.lock()->SetPosDeviation(Vector3(0.0f, 0.0f , 0.0f));
		pCollider.lock()->SetCollisionType(ObjectType::BOSSATTACK1, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::BOSSATTACK2, CollisionType::OBB);
		pCollider.lock()->SetCollisionType(ObjectType::BOSSWITCH, CollisionType::BC);
		pCollider.lock()->SetCollisionType(ObjectType::STAGE, CollisionType::AABB);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE_NORTH, CollisionType::AABB);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE_WEST, CollisionType::AABB);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE_EAST, CollisionType::AABB);
		pCollider.lock()->SetCollisionType(ObjectType::OUTSIDE_SOUTH, CollisionType::AABB);
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("character");
	}
	if (!pPlayer.expired() && !pBBR.expired())
	{
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
		pGKB.lock()->SetKeyInfo(KeyBind::HEAL, KeyType::RELEASE, VK_Q, Delegate<Player, void>::CreateDelegator(pPlayer, &Player::EnableHeal), Delegate<Mouse, const bool>::CreateDelegator(m_pMouse, &Mouse::IsNotHitObject));
		pGKB.lock()->SetKeyInfo(KeyBind::TELEPORT, KeyType::RELEASE, VK_E, Delegate<Player, void>::CreateDelegator(pPlayer, &Player::EnableTeleport), Delegate<Mouse, const bool>::CreateDelegator(m_pMouse, &Mouse::IsNotHitObject));
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreatePlayerMagic()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::PLAYERATTACK);
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.1f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("sphere");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
		pRenderer3D.lock()->SetEmissiveColor(Vector4(35.f / 255.f, 105.f / 255.f, 180.f / 255.f, 1.f));
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
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<MasterWitch> pMasterWitch = pObj->AddComponent<MasterWitch>();
	std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	pObj->AddComponent<Event>().lock()->type.set(Event_Type::TALK_1);
	pObj->AddComponent<Event>().lock()->type.set(Event_Type::TALK_2);
	pObj->AddComponent<EventTrigger>().lock()->type.set(Event_Type::TALK_1);
	pObj->GetComponent<EventTrigger>().lock()->Cause();
	pObj->AddComponent<EventTrigger>().lock()->type.set(Event_Type::TALK_2);
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
		pBBR.lock()->ZaxisUnlock();
		pBBR.lock()->XaxisLock();	
		pBBR.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pBBR.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
		pBBR.lock()->SetCamera(m_pCamera);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBossWitchMagicBullet()
{
	Object::OWNER_OBJ pObj(new Object());
	pObj->SetType(ObjectType::BOSSATTACK1);
	pObj->DisableActive();
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBullet> pMB = pObj->AddComponent<MagicBullet>();
	std::weak_ptr<Renderer3D> pRenderer3D = pObj->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pObj->AddComponent<Mesh>();
	std::weak_ptr<Collider> pCollider = pObj->AddComponent<Collider>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localscale = 0.1f;
	}
	if (!pMesh.expired())
	{
		pMesh.lock()->Set("sphere");
	}
	if (!pRenderer3D.expired())
	{
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
		pRenderer3D.lock()->SetEmissiveColor(Vector4(210.f / 255.f, 165.f / 255.f, 100.f / 255.f, 1.f));
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
	m_pObjectList.emplace_back(pObj);

	std::weak_ptr<Transform> pTransform = pObj->GetComponent<Transform>();
	std::weak_ptr<MagicBall> pMB = pObj->AddComponent<MagicBall>();
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
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_EFFECT);
		pRenderer3D.lock()->SetEmissiveColor(Vector4(210.f / 255.f, 165.f / 255.f, 100.f / 255.f, 1.f));
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
	m_pObjectList.emplace_back(pObj);

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
		//pRenderer3D.lock()->EnableWrite(WriteType::DEPTH_OF_SHADOW);
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_CHARACTER);
	}
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateBoss1Object()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateEventObject()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);
	pObj->AddComponent<Event>();
	return pObj;
}

Object::WORKER_OBJ FactoryMethod::CreateMagicUI()
{
	Object::OWNER_OBJ pObj(new Object());
	m_pObjectList.emplace_back(pObj);
	RenderPipeline::GetInstance().AddRenderer(pObj->AddComponent<Renderer2D>());
	pObj->GetComponent<Renderer2D>().lock()->EnableDraw(DrawType::UI_MAGIC);
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
		pObjectList.emplace_back(m_pObjectList[i]);
	}
	m_pObjectList.clear();
}
