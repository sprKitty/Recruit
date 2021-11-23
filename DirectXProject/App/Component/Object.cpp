#include "Object.h"
#include "Component.h"
#include <Transform.h>
#include <memory>

Object::Object()
{
}

Object::~Object()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		com.reset();
	}
	ComponentList.clear();
}

void Object::Init()
{
	shared_from_this()->AddComponent<Transform>();
	auto buff = ComponentList;
	for (auto com : buff)
	{
		com->Init();
	}
}

void Object::Uninit()
{
	auto buff = ComponentList;
	for (auto com : buff)
	{
		com->Uninit();
	}
}

void Object::Update()
{
	auto buff = ComponentList;
	for (auto com : buff)
	{
		com->Update();
	}
}