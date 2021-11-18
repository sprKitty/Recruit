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

	inline void EnablePlay()
	{
		m_isPlay = true;
	}

	void ResetEvent();

	inline const bool IsFinishAll()const
	{
		return m_isFinishAll;
	}

	template<class T>
	inline void Add(const std::shared_ptr<T>& pT)
	{
		m_pActionEvents.push_back(std::move(pT));
	}

private:
	bool m_isPlay;
	bool m_isFinishAll;
	std::vector<std::shared_ptr<EventBase> > m_pActionEvents;
	int m_nArray;
};