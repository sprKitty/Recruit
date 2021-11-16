#include "ParallelLightMgr.h"
#include <Shader/ShaderBuffer.h>
#include <App/Light.h>
#include <App/Camera.h>
#include <MyMath.h>
#include <Defines.h>

ParallelLightMgr::ParallelLightMgr()
	:m_pNearLight(nullptr)
	, m_pMediumLight(nullptr)
	, m_pFarLight(nullptr)
{
}

ParallelLightMgr::~ParallelLightMgr()
{
	Release();
}

void ParallelLightMgr::Init()
{
	float length = 100;
	Vector2 vNearFar;
	Vector3 direction = Vector3(1.0f, 1.0f, -1.0f);
	Vector3 directionPos = direction * length;
	Vector3 pos[3] =
	{
		Vector3(directionPos * 10.0f),
		Vector3(directionPos * 20.0f),
		Vector3(directionPos * 40.0f),
	};
	vNearFar = Vector2(0.1f, pos[0].y * 2.0f);
	m_pNearLight.reset(new Light());
	m_pNearLight->Init(&pos[0], nullptr, &vNearFar, Vector2(0, 0.000f), 0.4f, 1.0f);

	vNearFar = Vector2(pos[0].y * 0.5f, pos[1].y * 2.0f);
	m_pMediumLight.reset(new Light());
	m_pMediumLight->Init(&pos[1], nullptr, &vNearFar, Vector2(0, -0.0000f), 0.4f, 1.0f);

	vNearFar = Vector2(pos[1].y * 0.5f, pos[2].y * 2.0f);
	m_pFarLight.reset(new Light());
	m_pFarLight->Init(&pos[2], nullptr, &vNearFar, Vector2(0, -0.0000f), 0.5f, 1.0f);
}

void ParallelLightMgr::Release()
{
}

void ParallelLightMgr::Update()
{
	m_pNearLight->Update();
	m_pMediumLight->Update();
	m_pFarLight->Update();
}

void ParallelLightMgr::Bind(Type type)
{
	switch (type)
	{
	case Type::T_NEAR:
		m_pNearLight->Bind();

		break;
	case Type::T_MEDIUM:
		m_pMediumLight->Bind();

		break;
	case Type::T_FAR:
		m_pFarLight->Bind();

		break;
	case Type::T_MAX:
		ShaderBuffer::GetInstance().SetLightView(m_pNearLight->GetView());
		ShaderBuffer::GetInstance().SetMediumLightView(m_pMediumLight->GetView());
		ShaderBuffer::GetInstance().SetFarLightView(m_pFarLight->GetView());
		ShaderBuffer::GetInstance().SetLightProj(m_pNearLight->GetProj());
		ShaderBuffer::GetInstance().SetMediumLightProj(m_pMediumLight->GetProj());
		ShaderBuffer::GetInstance().SetFarLightProj(m_pFarLight->GetProj());
		ShaderBuffer::GetInstance().SetLightPos(XMFLOAT4(m_pMediumLight->GetPos().x, m_pMediumLight->GetPos().y, m_pMediumLight->GetPos().z, 0));
		m_pNearLight->UpdateValue(0);
		m_pMediumLight->UpdateValue(1);
		m_pFarLight->UpdateValue(2);
		break;

	default:
		break;
	}
	ShaderBuffer::GetInstance().Write(CB_VP);
}