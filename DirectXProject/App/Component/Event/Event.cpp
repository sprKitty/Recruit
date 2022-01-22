#include "Event.h"
#include <App/EventMgr.h>
#include <App/Event/EventBase.h>

Event::Event()
{
	type = Property<Event_Type::Kind>(&m_type);
}

Event::~Event()
{
}

void Event::Init()
{
	EventMgr::GetInstance().AddEvent(weak_from_this());
	m_isPlay = false;
	m_isFinishAll = false;
	m_nArray = 0;
	m_type = Event_Type::MAX;
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
	EventMgr::GetInstance().ReleaseEvent(weak_from_this());
}

void Event::Update()
{
	if (!m_isPlay)return; // �C�x���g�����s�ł�������������Ă��邩
	if (m_isFinishAll)return; // �C�x���g�����s�ς݂�

	if (m_pActionEvents[m_nArray]->IsFin()) // ���݂̃C�x���g���I�������
	{
		int nSize = static_cast<int>(m_pActionEvents.size());
		if (m_nArray >= nSize - 1)
		{
			m_isFinishAll = true;
			return;
		}
		else
		{
			++m_nArray;
		}
	}

	m_pActionEvents[m_nArray]->Update();
}

void Event::ResetEvent()
{
	m_isPlay = false;
	m_isFinishAll = false;
	for (const auto itr : m_pActionEvents)
	{
		itr->Init();
	}
}
