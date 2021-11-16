#pragma once
#include <System/ClassDesign/Singleton.h>


class DebugLog : Singleton<DebugLog>
{
public:
	friend class Singleton<DebugLog>;

protected:
	DebugLog() {}
	~DebugLog()override {}

	void Initialize()override;
	void Finalize()override;

private:

};