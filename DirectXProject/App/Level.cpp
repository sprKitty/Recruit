#include "Level.h"
#include <App/FactoryMethod.h>
#include <App/Scene/SceneBase.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Transform.h>
#include <App/Component/Mesh.h>
#include <App/Component/Instancing.h>


void Level::Create(std::weak_ptr<SceneBase> pScene)
{
	if (pScene.expired())return;

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
	pScene.lock()->AddObj(pTerrain);

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
}
