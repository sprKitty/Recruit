#pragma once

class EventBase
{
public:
	EventBase() 
		: m_isFin(false)
	{
	}
	virtual ~EventBase() {}

	virtual void Load(const char*) = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsFin() { return m_isFin; }

protected:
	bool m_isFin;

private:

};