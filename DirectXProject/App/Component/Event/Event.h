#pragma once
#include <App/Component/Component.h>
#include <memory>

class EventBase;

class Event : public Component
{
public:
	Event();
	~Event()override;

	void Init()override;
	void Uninit()override;
	void Update()override;

	void EnablePlay()
	{
		m_isPlay = true;
	}

	template<class T>
	void Add(const std::shared_ptr<T>& pT)
	{
		m_pActionEvents.push_back(std::move(std::dynamic_pointer_cast<EventBase>(pT)));
	}

private:
	bool m_isPlay;
	std::vector<std::shared_ptr<EventBase> > m_pActionEvents;
	std::vector<std::shared_ptr<EventBase> >::iterator m_pActiveItr;
};