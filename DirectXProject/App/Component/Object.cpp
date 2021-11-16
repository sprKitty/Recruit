#include "Object.h"
#include "Component.h"
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
	m_Transform.move = 0;
	auto buff = ComponentList;
	for (auto com : buff)
	{
		com->Update();
	}
	if (m_Transform.rot.x > 180)m_Transform.rot.x -= 360;
	if (m_Transform.rot.x < -180)m_Transform.rot.x += 360;

	if (m_Transform.rot.y > 180)m_Transform.rot.y -= 360;
	if (m_Transform.rot.y < -180)m_Transform.rot.y += 360;

	if (m_Transform.rot.z > 180)m_Transform.rot.z -= 360;
	if (m_Transform.rot.z < -180)m_Transform.rot.z += 360;
}