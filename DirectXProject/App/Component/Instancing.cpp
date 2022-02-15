#include "Instancing.h"
#include <App/Component/Object.h>
#include <App/Component/Transform.h>
#include <App/MeshData.h>
#include <System/DrawBuffer.h>
#include <System/DebugLog.h>
#include <Shader/ShaderBuffer.h>


void Instancing::Init()
{
	scale.set(1.0f);
	angle.set(0.0f);
}

void Instancing::Uninit()
{
}

void Instancing::Update()
{
}

void Instancing::Bind(const std::weak_ptr<ShaderBuffer>& pBuf)
{
	if (m_pDrawBuffer.expired())return;
	if (pBuf.expired())return;
	pBuf.lock()->SetInstancingWorld(scale.get(), angle.get(), m_vPosList);
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
	VectorInt3 vCalcMarge(vMarginError.x * 2.0f * 100.0f, vMarginError.y * 2.0f * 100.0f, vMarginError.z * 2.0f * 100.0f);
	if (vNum.x != 0)vSize.x /= vNum.x;
	if (vNum.y != 0)vSize.y /= vNum.y;
	if (vNum.z != 0)vSize.z /= vNum.z;
	Vector3 vPos = minLeft;
	Vector3 vMarge = 0;
	for (int x = 0; x < vNum.x; ++x)
	{
		for (int y = 0; y < vNum.y; ++y)
		{
			for (int z = 0; z < vNum.z; ++z)
			{
				if (vCalcMarge.x != 0)vMarge.x = rand() % vCalcMarge.x;
				if (vCalcMarge.y != 0)vMarge.y = rand() % vCalcMarge.y;
				if (vCalcMarge.z != 0)vMarge.z = rand() % vCalcMarge.z;
				vPos.x = minLeft.x + vSize.x * x + (vSize.x * 0.5f) + (vMarge.x / 100.0f - vMarginError.x);
				vPos.y = minLeft.y + vSize.y * y + (vSize.y * 0.5f) + (vMarge.y / 100.0f - vMarginError.y);
				vPos.z = minLeft.z + vSize.z * z + (vSize.z * 0.5f) + (vMarge.z / 100.0f - vMarginError.z);
				m_vPosList.emplace_back(vPos);
			}
		}
	}
}