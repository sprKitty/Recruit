#include "Object.h"
#include "Component.h"
#include <App/Component/Transform.h>
#include <memory>

Object::Object()
{
	Init();
}

Object::~Object()
{
	Uninit();
	for (auto& itr : m_pComponentList)
	{
		itr.reset();
	}
	m_pComponentList.clear();
}

void Object::Init()
{
	AddComponent<Transform>();
	m_isDelete = false;
	m_isActive = true;
	m_type = ObjectType::NONE;
	for (const auto& itr : m_pComponentList)
	{
		itr->Init();
	}
}

void Object::Uninit()
{
	m_isActive = false;
	m_isDelete = true;
	for (const auto& itr : m_pComponentList)
	{
		itr->Uninit();
	}
}

void Object::Update()
{
	for (const auto& itr : m_pComponentList)
	{
		itr->Update();
	}
}