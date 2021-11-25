#pragma once
#include <System/ClassDesign/Delegate.h>
#include <System/ClassDesign/Manager.h>
#include <System/Input.h>
#include <vector>

namespace KeyType
{
	enum Kind
	{
		TRIGGER,
		PRESS,
		RELEASE,
		REPEAT,

		MAX
	};
}

namespace KeyBind
{
	enum Kind
	{
		MOVE,
		ATTACK,
		//SKILLATTACK1,
		//SKILLATTACK2,

		MAX,
	};
}

class GameKeyBind : public Manager
{
private:
	struct Info
	{
		KeyType::Kind type;
		int nKey;
		std::shared_ptr<DelegateBase<const bool> > pState;
		std::shared_ptr<DelegateBase<void> > pAction;
	};

public:
	GameKeyBind() {}
	~GameKeyBind() {}

	void Initialize();
	void Finalize();
	void Update();

	inline void SetKeyInfo(KeyBind::Kind bind, KeyType::Kind type, int key, const std::shared_ptr<DelegateBase<void> >&  pAction, const std::shared_ptr<DelegateBase<const bool> > pState = nullptr)
	{
		if (bind >= m_Info.size())return;

		m_Info[bind].type = type;
		m_Info[bind].nKey = key;
		m_Info[bind].pAction = pAction;
		if (pState)
		{
			m_Info[bind].pState = pState;
		}
	}

private:
	std::vector<Info> m_Info;
};