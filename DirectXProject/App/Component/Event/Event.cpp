#include "Event.h"
#include <App/Event/EventBase.h>

Event::Event()
{
}

Event::~Event()
{
}

void Event::Init()
{
	m_isPlay = false;
	m_isFinishAll = false;
	for (const auto& itr : m_pActionEvents)
	{
		itr->Init();
	}
}

void Event::Uninit()
{
	for (const auto& itr : m_pActionEvents)
	{
		itr->Uninit();
	}
}

void Event::Update()
{
	if (!m_isPlay)return;
	if (m_isFinishAll)return;

	if ((*m_pActiveItr)->IsFin())
	{
		if (m_pActiveItr == m_pActionEvents.end())
		{
			m_isFinishAll = true;
			return;
		}
		else
		{
			++m_pActiveItr;
		}

	}
	(*m_pActiveItr)->Update();
}

void Event::ResetEvent()
{
	m_isPlay = false;
	m_isFinishAll = false;
	for (const auto itr : m_pActionEvents)
	{
		itr->Init();
	}
	m_pActiveItr = m_pActionEvents.begin();
}
