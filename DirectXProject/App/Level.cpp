#include "Level.h"
#include <App/Scene/SceneBase.h>
#include <App/Component/Renderer/Renderer3D.h>

void Level::Create(std::weak_ptr<SceneBase> pScene)
{
	if (pScene.expired())return;

	Object::OWNER_OBJ pLand(new Object());
	pLand->SetType(Object::Type::LEVEL);
	Transform t;
	t.scale.x = 10.0f;
	t.scale.z = 10.0f;
	pLand->SetTransform(t);
	std::weak_ptr<Renderer3D> pRenderer3D = pLand->AddComponent<Renderer3D>();
	pRenderer3D.lock()->SetModel("Assets/OBJ/field2.obj");
	pRenderer3D.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);

	pScene.lock()->AddObj(pLand);
}
