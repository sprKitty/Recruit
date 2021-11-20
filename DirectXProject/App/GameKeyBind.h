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
		NORMALATTACK,
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
		std::shared_ptr<DelegateBase_void<void> > pAction;
	};

public:
	GameKeyBind() {}
	~GameKeyBind() {}

	void Initialize();
	void Finalize();
	void Update();

	inline void SetKeyInfo(KeyBind::Kind bind, KeyType::Kind type, int key, const std::shared_ptr<DelegateBase_void<void> >&  pFunc)
	{
		if (bind >= m_Info.size())return;

		m_Info[bind].type = type;
		m_Info[bind].nKey = key;
		m_Info[bind].pAction = pFunc;
	}

private:
	std::vector<Info> m_Info;
};