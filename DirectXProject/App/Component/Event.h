#pragma once
#include <App/Component/Component.h>


class EventBase;

class Event : public Component
{
public:
	Event();
	~Event()override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void EnablePlay()
	{
		m_isPlay = true;
	}

	void Add(std::shared_ptr<EventBase>& pEB)
	{
		m_pActionEvents.push_back(std::move(pEB));
	}

private:
	bool m_isPlay;
	std::vector<std::shared_ptr<EventBase> > m_pActionEvents;
	std::vector<std::shared_ptr<EventBase> >::iterator m_pActiveItr;
};