#pragma once
#include <System/ClassDesign/Singleton.h>
#include <string>

class Clocker : public Singleton<Clocker>
{
private:
	friend class Singleton<Clocker>;

public:
	void Initialize()override;
	void Finalize()override;

	void StartFrame(long start);
	void EndFrame(long end);
	void CalcFrameTime();
	const std::string GetDateTimeStr();

	const double& GetGameTime();
	inline const float GetFrameTime() { return static_cast<float>(m_dFrameTime); }
protected:
	Clocker() {}
	virtual ~Clocker() {}

private:
	void AddTime();

private:
	long  m_lStart;
	long  m_lEnd;
	double m_dFrameTime;
	double m_dGameTime;

};