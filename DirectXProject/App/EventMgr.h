#pragma once
#include <System/ClassDesign/Singleton.h>
#include <System/ClassDesign/Delegate.h>
#include <App/Component/Event/EventTrigger.h>
#include <App/Component/Object.h>

class Event;

class EventMgr : public Singleton<EventMgr>
{
public:
	friend class Singleton<EventMgr>;

public:
	void Initialize()override {}
	void Finalize()override;
	void SetEventInfo(const std::weak_ptr<Event> pEvent, const Object::WORKER_OBJECTLIST& pObjects, const EventTrigger::Type& type);

	void CallFunc();
private:
	struct EventInfo
	{
		std::shared_ptr<DelegateBase_void<void> > pEvent;
		std::vector<std::shared_ptr<DelegateBase_void<bool> > > pEventTriggers;
	};

	EventMgr();
	~EventMgr();

private:
	std::vector<EventInfo> m_EventInfos;
};