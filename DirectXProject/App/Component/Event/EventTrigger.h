#pragma once
#include <App/Component/Component.h>

class EventTrigger: public Component
{
public:
	enum class Type
	{
		TALK_1,
		CAMERASHAKE_1,

		MAX,
	};

public:
	EventTrigger();
	~EventTrigger()override;

	void Init()override;
	void Uninit()override;
	void Update()override;

	/*
	* @brief trigger‚ğ—LŒøó‘Ô‚É‚·‚é
	*/
	inline void Cause() { m_isCaused = true; }

	inline const bool Check() { return m_isCaused; }

	inline  const Type GetType() { return m_type; }
	inline void SetType(const Type type) { m_type = type; }

private:
	Type m_type;
	bool m_isCaused; // trigger‚ª—LŒø‚©
};