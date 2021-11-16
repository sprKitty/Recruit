#include "Clocker.h"
#include <iomanip>
#include <sstream>

void Clocker::Initialize()
{
	m_lStart = 0;
	m_lEnd = 0;
	m_dFrameTime = 0.0f;
	m_dGameTime = 0.0f;
}

void Clocker::Finalize()
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

const std::string Clocker::GetDateTimeStr()
{
	time_t pTime = time(nullptr);
	const tm* localT = localtime(&pTime);
	std::stringstream s;
	s << "20" << localT->tm_year - 100 << "”N";
	s << std::setw(2) << std::setfill('0') << localT->tm_mon + 1 << "ŒŽ";
	s << std::setw(2) << std::setfill('0') << localT->tm_mday << "“ú";
	s << std::setw(2) << std::setfill('0') << localT->tm_hour << "Žž";
	s << std::setw(2) << std::setfill('0') << localT->tm_min << "•ª";
	s << std::setw(2) << std::setfill('0') << localT->tm_sec << "•b";

	return s.str();
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
