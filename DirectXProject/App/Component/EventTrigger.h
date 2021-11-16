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
	void Draw()override;

	bool Check();

	Type& GetType() { return m_type; }
	void SetType(Type type) { m_type = type; }

private:
	Type m_type;
};