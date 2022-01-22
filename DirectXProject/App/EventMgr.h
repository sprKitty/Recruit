#pragma once
#include <System/ClassDesign/Singleton.h>
#include <System/ClassDesign/Delegate.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Event/EventTrigger.h>
#include <App/Component/Object.h>


class EventMgr : public Singleton<EventMgr>
{
public:
	friend class Singleton<EventMgr>;

public:
	void Initialize()override;
	void Finalize()override;
	void CompositionEventInfo(const Event_Type::Kind type);

	void Update();

	void AddEvent(const std::weak_ptr<Component>& pComponent);
	void ReleaseEvent(const std::weak_ptr<Component>& pComponent);

	void AddEventTrigger(const std::weak_ptr<Component>& pComponent);
	void ReleaseEventTrigger(const std::weak_ptr<Component>& pComponent);


private:
	struct EventInfo
	{
		std::weak_ptr<Event> pEvent;
		weak_ptr_list<EventTrigger> pEventTriggerList;
	};

	EventMgr();
	~EventMgr()override;

private:
	std::vector<EventInfo> m_EventInfos;
	weak_ptr_list<Event> m_pEventList;
	weak_ptr_list<EventTrigger> m_pEventTriggerList;
};