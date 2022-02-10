#include "EventMgr.h"
#include <App/Component/Object.h>
#include <App/Component/Event/Event.h>
#include <Defines.h>

EventMgr::EventMgr()
{
}

EventMgr::~EventMgr()
{
}

void EventMgr::CompositionEventInfo(const Event_Type::Kind type)
{
	m_EventInfos[type].pEventTriggerList.clear();
	m_EventInfos[type].pEvent.reset();
	for (const auto& itrEvent : m_pEventList)
	{
		if (itrEvent.expired())continue;
		if (itrEvent.lock()->type.get() != type)continue;

		for (auto itrEventTrigger : m_pEventTriggerList)
		{
			if (itrEventTrigger.expired())continue;
			if (itrEventTrigger.lock()->type.get() != type)continue;
			m_EventInfos[type].pEventTriggerList.emplace_back(itrEventTrigger);
		}
		m_EventInfos[type].pEvent = itrEvent;
	}
}

void EventMgr::Update()
{
	for (std::vector<EventInfo>::const_iterator itrEI = m_EventInfos.begin(); itrEI != m_EventInfos.end(); ++itrEI)
	{
		if (itrEI->pEvent.expired())
		{
			continue;
		}
		bool isClear = true;
		for (auto itrET : itrEI->pEventTriggerList)
		{
			if (itrET.expired())
			{
				itrEI = m_EventInfos.erase(itrEI);
				break;
			}

			isClear = itrET.lock()->Check();
			if (!isClear)break;

		}
		if (isClear) // イベント実行
		{
			if (!itrEI->pEvent.expired())
			{
				itrEI->pEvent.lock()->EnablePlay();
			}
		}
	}
}

void EventMgr::AddEvent(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Event> pEvent = std::dynamic_pointer_cast<Event>(pComponent.lock());
	if (pEvent.expired())return;
	m_pEventList.emplace_back(pEvent);
}

void EventMgr::ReleaseEvent(const std::weak_ptr<Component>& pComponent)
{
	if (pComponent.expired())return;

	for (weak_ptr_list<Event>::iterator itr = m_pEventList.begin(); itr != m_pEventList.end();)
	{
		if (itr->expired())continue;

		if (itr->lock() == pComponent.lock())
		{
			itr = m_pEventList.erase(itr);
		}

		if (itr == m_pEventList.end())break;

		++itr;
	}
}

void EventMgr::AddEventTrigger(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<EventTrigger> pEventTrigger = std::dynamic_pointer_cast<EventTrigger>(pComponent.lock());
	if (pEventTrigger.expired())return;
	m_pEventTriggerList.emplace_back(pEventTrigger);
}

void EventMgr::ReleaseEventTrigger(const std::weak_ptr<Component>& pComponent)
{
	if (pComponent.expired())return;

	for (weak_ptr_list<EventTrigger>::iterator itr = m_pEventTriggerList.begin(); itr != m_pEventTriggerList.end();)
	{
		if (itr->expired())continue;

		if (itr->lock() == pComponent.lock())
		{
			itr = m_pEventTriggerList.erase(itr);
		}

		if (itr == m_pEventTriggerList.end())break;

		++itr;
	}
}

void EventMgr::Initialize()
{
	m_EventInfos.resize(Event_Type::MAX);
}

void EventMgr::Finalize()
{
	m_EventInfos.clear();
}
