#include "LV_StageBranch.h"
#include <App/FactoryMethod.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Transform.h>
#include <App/Component/Mesh.h>
#include <App/Component/Instancing.h>

void LV_StageBranch::Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)
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
	pScene.lock()->AddObject(pTerrain);
}

void LV_StageBranch::Finalize(const Object::WORKER_OBJ pObject)
{
}
