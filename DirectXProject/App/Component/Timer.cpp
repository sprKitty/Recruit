#include "Timer.h"
#include <Shader/ShaderBuffer.h>

void Timer::Init()
{
}

void Timer::Uninit()
{
}

void Timer::Update()
{
	m_isLimit = false;
	if (m_fCount > SYSTEM_MAXCOUNT)
	{
		m_fCount -= SYSTEM_MAXCOUNT;
		return;
	}
	if (m_fCount > m_fMAXCount)
	{
		m_fCount -= m_fMAXCount;
		m_isLimit = true;
	}
}

void Timer::Draw()
{
}
