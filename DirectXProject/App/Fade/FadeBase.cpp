#include "FadeBase.h"

FadeBase::FadeBase()
{
	inOutSpeed = Property<float>(&m_fInOutSpeed);
	time = Property<float>(&m_fTime);
}

FadeBase::~FadeBase()
{
}

const bool FadeBase::Update()
{
	if (m_isIn)
	{
		if (m_fTime >= m_fInOutSpeed)
		{
			m_isIn = false;
			m_fTime = m_fInOutSpeed;
			return false;
		}
		InUpdate();
		return true;
	}
	else if (m_isOut)
	{
		if (m_fTime <= 0.0f)
		{
			m_isOut = false;
			m_fTime = 0.0f;
			return false;
		}
		OutUpdate();
		return true;
	}
	return false;
}

void FadeBase::StartFadeIn()
{
	m_isIn = true;
	m_isOut = false;
}

void FadeBase::StartFadeOut()
{
	m_isIn = false;
	m_isOut = true;
}

void FadeBase::InUpdate()
{
	m_fTime += Clocker::GetInstance().DeltaTime();
	m_fInOut = m_fTime / m_fInOutSpeed;
}

void FadeBase::OutUpdate()
{
	m_fTime -= Clocker::GetInstance().DeltaTime();
	m_fInOut = m_fTime / m_fInOutSpeed;
}
