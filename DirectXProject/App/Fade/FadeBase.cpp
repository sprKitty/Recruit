#include "FadeBase.h"

FadeBase::FadeBase()
{
	//inOutSpeed = Property<float>(&m_fInOutSpeed);
	//time = Property<float>(&m_fTime);
}

FadeBase::~FadeBase()
{
}

const bool FadeBase::Update()
{
	float fInOutSpeed = inOutSpeed.get();
	float fTime = time.get();
	if (m_isIn)
	{
		if (fTime >= fInOutSpeed)
		{
			m_isIn = false;
			fTime = fInOutSpeed;
			time.set(fTime);
			return false;
		}
		InUpdate();
		return true;
	}
	else if (m_isOut)
	{
		if (fTime <= 0.0f)
		{
			m_isOut = false;
			fTime = 0.0f;
			time.set(fTime);
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
	time.set(time.get() + Clocker::GetInstance().DeltaTime());
	m_fInOut = time.get() / inOutSpeed.get();
}

void FadeBase::OutUpdate()
{
	time.set(time.get() - Clocker::GetInstance().DeltaTime());
	m_fInOut = time.get() / inOutSpeed.get();
}
