#include "Clocker.h"


void Clocker::Init()
{
	m_lStart = 0;
	m_lEnd = 0;
	m_dFrameTime = 0.0f;
	m_dGameTime = 0.0f;
}

void Clocker::Uninit()
{
}

void Clocker::StartFrame(long start)
{
	m_lStart = start;
}

void Clocker::EndFrame(long end)
{
	m_lEnd = end;
}

void Clocker::CalcFrameTime()
{
	long lFrame = m_lEnd - m_lStart;
	m_dFrameTime = static_cast<double>(lFrame) / 1000.0f;
	AddTime();
}

const double& Clocker::GetGameTime()
{
	return m_dGameTime;
}

const double & Clocker::GetFrameTime()
{
	return m_dFrameTime;
}

void Clocker::AddTime()
{
	m_dGameTime += m_dFrameTime;
}
