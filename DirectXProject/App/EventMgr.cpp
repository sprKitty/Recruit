#include "EventMgr.h"
#include <App/Component/Event/Event.h>
#include <Defines.h>
#include <App/Component/Object.h>

EventMgr::EventMgr()
{
}

EventMgr::~EventMgr()
{
}

void EventMgr::SetEventInfo(const std::weak_ptr<Event> pEvent, const Object::WORKER_OBJECTLIST& pObjects, const EventTrigger::Type & type)
{
	if (PTRNULLCHECK(pEvent))return;
	EventInfo ei;

	for (auto itr : pObjects)
	{
		if (itr.expired())continue;
	
		std::weak_ptr<EventTrigger> pEventTrigger = itr.lock()->GetComponent<EventTrigger>();
		if (pEventTrigger.expired())continue;
		
		if (pEventTrigger.lock()->GetType() != type)continue;
	
		std::shared_ptr<DelegateBase<const bool> > pDelegate = Delegate<EventTrigger,const bool>::CreateDelegator(pEventTrigger, &EventTrigger::Check);
		ei.pEventTriggers.emplace_back(pDelegate);
	}
	ei.pEvent = pEvent;

	m_EventInfos.push_back(ei);
}

void EventMgr::CallFunc()
{
	for (std::vector<EventInfo>::const_iterator itrEI = m_EventInfos.begin(); itrEI != m_EventInfos.end();)
	{
		bool clear = true;
		for (auto itrET : itrEI->pEventTriggers)
		{
			if (PTRNULLCHECK(itrET))
			{
				itrEI = m_EventInfos.erase(itrEI);
				break;
			}

			clear = itrET->Execute();
			if (clear != 1)break;

		}
		if (clear) // イベント実行
		{
			if (!itrEI->pEvent.expired())
			{
				itrEI->pEvent.lock()->EnablePlay();
			}
		}
		if (itrEI == m_EventInfos.end())break;
		++itrEI;
	}
}

void EventMgr::Finalize()
{
	m_EventInfos.clear();
}
