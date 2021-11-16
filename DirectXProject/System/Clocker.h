#pragma once
#include <System/ClassDesign/Singleton.h>

class Clocker : public Singleton<Clocker>
{
private:
	friend class Singleton<Clocker>;

public:
	void Init();
	void Uninit();

	void StartFrame(long start);
	void EndFrame(long end);
	void CalcFrameTime();

	const double& GetGameTime();
	const double& GetFrameTime();
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