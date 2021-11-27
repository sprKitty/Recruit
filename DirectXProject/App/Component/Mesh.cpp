#include "Mesh.h"
#include <App/MeshData.h>
#include <System/DrawBuffer.h>
#include <System/DebugLog.h>


void Mesh::Init()
{
	m_primitiveType = PrimitiveType::TRIANGLELIST;
}

void Mesh::Uninit()
{
}

void Mesh::Update()
{
}

void Mesh::Bind()
{
	if (m_pDrawBuffer.expired())return;

	switch (m_primitiveType)
	{
	case PrimitiveType::TRIANGLELIST:
		m_pDrawBuffer.lock()->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case PrimitiveType::TRIANGLESTRIP:
		m_pDrawBuffer.lock()->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	case PrimitiveType::MAX:
		break;
	default:
		break;
	}
}

void Mesh::Set(const std::string str)
{
	MeshData::Info info = MeshData::GetInstance().Get(str);
	if (info.pDrawBuffer.use_count() > 0)
	{
		m_pDrawBuffer = info.pDrawBuffer;
	}

	if (!info.surfaceList.empty())
	{
		m_surfaceList = info.surfaceList;
	}
	
	m_primitiveType = info.type;
}
