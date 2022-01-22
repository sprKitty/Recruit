#pragma once
#include <App/Component/Component.h>
#include <System/ClassDesign/Property.h>

namespace Event_Type
{
	enum Kind
	{
		TALK_1,
		TALK_2,

		MAX,
	};
}

class EventTrigger: public Component
{
public:
	EventTrigger();
	~EventTrigger()override;

	void Init()override;
	void Uninit()override;
	void Update()override;

	/*
	* @brief triggerを有効状態にする
	*/
	inline void Cause() { m_isCaused = true; }

	inline const bool Check() { return true; }

public:
	Property<Event_Type::Kind> type;

private:
	Event_Type::Kind m_type;
	bool m_isCaused; // triggerが有効か
};