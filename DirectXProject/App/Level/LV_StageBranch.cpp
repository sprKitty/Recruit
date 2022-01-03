#include "LV_StageBranch.h"
#include <App/FactoryMethod.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Transform.h>
#include <App/Component/Mesh.h>
#include <App/Component/Instancing.h>

void LV_StageBranch::Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)
{
	if (pScene.expired())return;
	if (pObject.expired())return;

	Object::OWNER_OBJ pTerrain;
	pTerrain.reset(new Object());
	pTerrain->SetType(ObjectType::STAGE);
	std::weak_ptr<Transform> pTransform = pTerrain->GetComponent<Transform>();
	std::weak_ptr<Renderer3D> pRenderer3D = pTerrain->AddComponent<Renderer3D>();
	std::weak_ptr<Mesh> pMesh = pTerrain->AddComponent<Mesh>();
	if (!pMesh.expired() && !pRenderer3D.expired() && !pTransform.expired())
	{
		pTransform.lock()->localscale.x = 20.0f;
		pTransform.lock()->localscale.y = 20.0f;
		pTransform.lock()->localscale.z = 20.0f;
		pMesh.lock()->Set("branchField.obj");
		pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_TRIPLANAR);
		pRenderer3D.lock()->SetMainImage("terrain");
		pRenderer3D.lock()->SetBumpImage("terrainBump");
	}
	pScene.lock()->AddObject(pTerrain);

	Object::WORKER_OBJ pOutSideArea;
	std::weak_ptr<Instancing> pInstancing;
	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 27.5f,0.0f,40.0f };
		pTransform.lock()->localscale = { 45.0f,1.0f,50.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 15), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -27.5f,0.0f,40.0f };
		pTransform.lock()->localscale = { 45.0f,1.0f,50.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 15), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 27.5f,0.0f,-40.0f };
		pTransform.lock()->localscale = { 45.0f,1.0f,50.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 15), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -27.5f,0.0f,-40.0f };
		pTransform.lock()->localscale = { 45.0f,1.0f,50.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 15), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 32.5f,0.0f,10.0f };
		pTransform.lock()->localscale = { 40.0f,1.0f,10.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 3), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -32.5f,0.0f,10.0f };
		pTransform.lock()->localscale = { 40.0f,1.0f,10.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 3), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 32.5f,0.0f,-10.0f };
		pTransform.lock()->localscale = { 40.0f,1.0f,10.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 3), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateOutsideArea();
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { -32.5f,0.0f,-10.0f };
		pTransform.lock()->localscale = { 40.0f,1.0f,10.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(12, 1, 3), Vector3(0.2f, 0, 0.2f));
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);

	pOutSideArea = FactoryMethod::GetInstance().CreateTransitionLevel();
	pOutSideArea.lock()->SetType(ObjectType::OUTSIDE_NORTH);
	pTransform = pOutSideArea.lock()->GetComponent<Transform>();
	pInstancing = pOutSideArea.lock()->GetComponent<Instancing>();
	if (!pTransform.expired())
	{
		pTransform.lock()->localpos = { 0.0f,0.0f,50.0f };
		pTransform.lock()->localscale = { 10.0f,10.0f,30.0f };
		if (!pInstancing.expired())
		{
			Vector3 vMin = pTransform.lock()->localpos - pTransform.lock()->localscale * 0.5f;
			Vector3 vMax = pTransform.lock()->localpos + pTransform.lock()->localscale * 0.5f;
			vMin.y = vMax.y = pTransform.lock()->localpos.y;
			pInstancing.lock()->SetRandomXYZ(vMin, vMax, VectorInt3(15, 1, 15), Vector3(0.6f, 0, 0.6f));
			pInstancing.lock()->scale.set(1.0f);
		}
	}
	m_pObjectList.emplace_back(pOutSideArea);
	pScene.lock()->MoveObject_FactoytoScene();
}

const Level_Type::Kind LV_StageBranch::Transition(const Object::WORKER_OBJ pObject)
{
	if (!pObject.expired())
	{
		std::weak_ptr<Collider> pCollider = pObject.lock()->GetComponent<Collider>();
		if (!pCollider.expired())
		{
			Collider::HitInfo info = pCollider.lock()->IsHitInfo(CollisionType::AABB);
			if (info.isFlg)
			{
				if (!info.pObj.expired())
				{
					ObjectType::Kind type = info.pObj.lock()->GetType();

					switch (type)
					{
					case ObjectType::OUTSIDE_NORTH:
						Finalize(pObject);
						return Level_Type::MASTERWITCH;

					case ObjectType::NONE:
					case ObjectType::PLAYER:
					case ObjectType::PLAYERATTACK:
					case ObjectType::BOSSWITCH:
					case ObjectType::BOSSATTACK1:
					case ObjectType::BOSSATTACK2:
					case ObjectType::STAGE:
					case ObjectType::OUTSIDE:
					case ObjectType::OUTSIDE_EAST:
					case ObjectType::OUTSIDE_WEST:
					case ObjectType::OUTSIDE_SOUTH:
					case ObjectType::MAX:
					default:
						break;
					}
				}
			}
		}
	}
	return Level_Type::MAX;
}

void LV_StageBranch::Finalize(const Object::WORKER_OBJ pObject)
{
}
