#include "Object.h"
#include "Component.h"
#include <App/Component/Transform.h>
#include <memory>

Object::Object()
{
}

Object::~Object()
{
	Uninit();
	for (auto com : ComponentList)
	{
		com.reset();
	}
	ComponentList.clear();
}

void Object::Init()
{
	shared_from_this()->AddComponent<Transform>();
	auto buff = ComponentList;
	m_isDelete = false;
	m_isActive = true;
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