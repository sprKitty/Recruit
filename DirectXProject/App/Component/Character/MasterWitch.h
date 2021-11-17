#pragma once
#include <App/Component/Character/Character.h>

class Object;

namespace Witch_State
{
	enum Kind
	{
		WAIT,

		MAX,
	};
}

class MasterWitch : public Character
{
public:
	MasterWitch() {}
	~MasterWitch()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline void SetTarget(const std::weak_ptr<Object>& pTarget)
	{
		m_pTarget = pTarget;
	}

private:
	void CalcTarget();

private:
	std::weak_ptr<Object> m_pTarget;
};