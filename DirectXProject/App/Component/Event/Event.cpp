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
	m_nArray = 0;
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
