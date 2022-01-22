#pragma once
#include <App/Component/Component.h>
#include <App/Component/Event/EventTrigger.h>
#include <System/ClassDesign/Property.h>
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

	inline void EnablePlay() { m_isPlay = true; }

	void ResetEvent();

	inline const bool IsFinishAll()const
	{
		if (m_isFinishAll)
		{
			int udh = 0;
		}
		return m_isFinishAll;
	}

	template<class T>
	inline void Add(const std::shared_ptr<T>& pT)
	{
		m_pActionEvents.emplace_back(pT);
	}

public:
	Property<Event_Type::Kind> type;

private:
	bool m_isPlay;
	bool m_isFinishAll;
	std::vector<std::shared_ptr<EventBase> > m_pActionEvents;
	int m_nArray;
	Event_Type::Kind m_type;
};