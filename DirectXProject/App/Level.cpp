#include "Level.h"
#include <App/Scene/SceneBase.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Transform.h>


void Level::Create(std::weak_ptr<SceneBase> pScene)
{
	if (pScene.expired())return;

	Object::OWNER_OBJ pLand(new Object());
	pLand->Init();
	pLand->SetType(ObjectType::LEVEL);
	std::weak_ptr<Transform> pTransform = pLand->GetComponent<Transform>();
	pTransform.lock()->localscale.x = 10.0f;
	pTransform.lock()->localscale.z = 10.0f;
	std::weak_ptr<Renderer3D> pRenderer3D = pLand->AddComponent<Renderer3D>();
	pRenderer3D.lock()->SetModel("Assets/OBJ/field2.obj");
	pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);

	pScene.lock()->AddObj(pLand);
}
