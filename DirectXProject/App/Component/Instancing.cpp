#include "Instancing.h"
#include <App/Component/Object.h>
#include <App/Component/Transform.h>
#include <App/MeshData.h>
#include <System/DrawBuffer.h>
#include <System/DebugLog.h>
#include <Shader/ShaderBuffer.h>


void Instancing::Init()
{
	m_vScale = 1.0f;
	m_vAngle = 0.0f;
}

void Instancing::Uninit()
{
}

void Instancing::Update()
{
}

void Instancing::Bind(const Vector3* vPos)
{
	if (m_pDrawBuffer.expired())return;
	if (vPos)
	{
		ShaderBuffer::GetInstance().SetInstancingWorld(*vPos, m_vAngle, m_vPosList);
	}
	else
	{
		ShaderBuffer::GetInstance().SetInstancingWorld(m_vScale, m_vAngle, m_vPosList);
	}
	switch (m_primitiveType)
	{
	case PrimitiveType::TRIANGLELIST:
		m_pDrawBuffer.lock()->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, static_cast<int>(m_vPosList.size()));
		break;
	case PrimitiveType::TRIANGLESTRIP:
		m_pDrawBuffer.lock()->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, static_cast<int>(m_vPosList.size()));
		break;
	case PrimitiveType::MAX:
		break;
	default:
		break;
	}
}

void Instancing::Set(const std::string str)
{
	MeshData::Info info = *MeshData::GetInstance().Get(str);
	if (info.pDrawBuffer.use_count() > 0)
	{
		m_pDrawBuffer = info.pDrawBuffer;
	}
	m_primitiveType = info.type;
}

void Instancing::SetRandomXYZ(const Vector3 & minLeft, const Vector3 & maxRight, const VectorInt3 & vNum, const Vector3 & vMarginError)
{
	m_vPosList.clear();
	float a = 0.5f - 0.5f;
	Vector3 vSize = maxRight - minLeft;
	if (vNum.x != 0)vSize.x /= vNum.x;
	if (vNum.y != 0)vSize.y /= vNum.y;
	if (vNum.z != 0)vSize.z /= vNum.z;
	Vector3 vPos = minLeft;
	for (int x = 0; x < vNum.x + 1; ++x)
	{
		vPos.x = minLeft.x + vSize.x * x;
		for (int y = 0; y < vNum.y + 1; ++y)
		{
			vPos.y = minLeft.y + vSize.y * y;
			for (int z = 0; z < vNum.z + 1; ++z)
			{
				vPos.z = minLeft.z + vSize.z * z;
				m_vPosList.push_back(vPos);
			}
		}
	}
}