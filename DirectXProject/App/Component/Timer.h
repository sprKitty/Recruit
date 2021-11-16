#pragma once
#include <App/Component/Component.h>


class Timer : public Component
{
public:
	Timer()
		: m_fCount(0)
		, m_fMAXCount(SYSTEM_MAXCOUNT)
		, m_isLimit(false)
	{

	}

	~Timer()
	{

	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	inline void AddCount(const float fCount) { m_fCount += fCount; }
	inline void SetMaxCount(float max) { m_fMAXCount = max; }

	inline float GetCount()const { return m_fCount; }
	inline bool IsLimit() { return m_isLimit; }

private:

	static constexpr int SYSTEM_MAXCOUNT = 100000000;
	
	bool m_isLimit;
	float m_fCount;
	float m_fMAXCount;
};