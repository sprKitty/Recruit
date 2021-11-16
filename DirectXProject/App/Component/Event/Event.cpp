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
	for (const auto itr : m_pActionEvents)
	{
		itr->Init();
	}
}

void Event::Uninit()
{
	for (const auto itr : m_pActionEvents)
	{
		itr->Uninit();
	}
}

void Event::Update()
{
	if (!m_isPlay)return;
	m_pActiveItr = m_pActionEvents.begin();
	if ((*m_pActiveItr)->IsFin())
	{
		++m_pActiveItr;
	}

	(*m_pActiveItr)->Update();
}