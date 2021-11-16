#include "Water.h"
#include "Object.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Timer.h"

Water::Water()
	: m_pWaveSpeed(nullptr)
{
}

Water::~Water()
{
}

void Water::Init()
{
	m_pWaveSpeed = m_pOwner->AddComponent<Timer>();
	m_pOwner->AddComponent<Renderer>()->SetModel("Assets/OBJ/water.obj");
	m_pOwner->GetComponent<Renderer>()->DrawWater();
	m_pOwner->AddressTransform().pos = Vector3(0, -25, 0);
	m_pOwner->AddressTransform().scale = Vector3(20, 20, 20);
}

void Water::Uninit()
{
}

void Water::Update()
{
	m_pWaveSpeed->AddCount(1.0f /60.0f);
	SHADER->SetWaterWaveTime(m_pWaveSpeed->GetCount());
}

void Water::Draw()
{
}
