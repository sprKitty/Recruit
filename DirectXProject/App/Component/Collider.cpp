#include "Collider.h"
#include "Object.h"
#include <App/Component/Mesh.h>
#include <App/Collision.h>
#include <Shader/ShaderBuffer.h>


void Collider::Init()
{
	Collision::GetInstance().AddCollider(weak_from_this());
	for (auto itr : m_typeList)
	{
		itr = false;
	}
	m_typeList[Collision_Type::AABB] = true;
}

void Collider::Uninit()
{
	Collision::GetInstance().ReleaseCollider(weak_from_this());
}

void Collider::Update()
{
	if (m_pMesh.expired())
	{
		m_pMesh = m_pOwner.lock()->GetComponent<Mesh>();
	}
}