#include "Talk.h"
#include <System/MessageWindow.h>
#include <System/LoadFiles.h>
#include <System/Clocker.h>


void Talk::Load(const char * pPath)
{
	LoadFiles lf;
	STRINGMAP sm = lf.CSV(pPath);

	if (!sm.empty())
	{
		for (const auto itr : sm)
		{
			Info info;
			for (int i = 1; i < itr.first.size(); i += 2)
			{
				std::string str = itr.first.substr(i, 2);
				info.vTitle.emplace_back(MessageWindow::GetOffset(str));
			}

			for (int i = 0; i < itr.second[0].size(); i += 2)
			{
				std::string str = itr.second[0].substr(i, 2);
				info.vMain.emplace_back(MessageWindow::GetOffset(str));
			}
			m_Infos.emplace_back(info);
		}
	}
}

void Talk::Init()
{
	m_isFin = false;
	m_nStep = 0;
	m_nShowNum = 1;
	m_fSpeed = 0.0f;
	m_fShowTime = MessageWindow::GetShowSpeed();
}

void Talk::Uninit()
{
}

void Talk::Update()
{
	m_pMSWindow.lock()->Bind(m_Infos[m_nStep].vTitle, m_Infos[m_nStep].vMain, 32, m_nShowNum);
	m_fSpeed += Clocker::GetInstance().DeltaTime();
	if (m_nShowNum < m_Infos[m_nStep].vMain.size())
	{
		if (m_fSpeed >= m_fShowTime)
		{
			++m_nShowNum;
			m_fSpeed -= m_fShowTime;
		}
	}
	else
	{
		if (m_fSpeed > 0.5f)
		{
			m_fSpeed = 0.0f;
			m_nShowNum = 1;
			if (m_nStep < m_Infos.size())
			{
				++m_nStep;
				if (m_nStep == m_Infos.size())
				{
					m_isFin = true;
					m_nStep = 0;
				}
			}
		}
	}
}

void Talk::Draw()
{
}
