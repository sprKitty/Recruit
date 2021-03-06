#include "EventTrigger.h"
#include <App/EventMgr.h>

EventTrigger::EventTrigger()
{
}

EventTrigger::~EventTrigger()
{
}

void EventTrigger::Init()
{
	EventMgr::GetInstance().AddEventTrigger(weak_from_this());
	m_isCaused = false;
}

void EventTrigger::Uninit()
{
	EventMgr::GetInstance().ReleaseEventTrigger(weak_from_this());
}

void EventTrigger::Update()
{
}