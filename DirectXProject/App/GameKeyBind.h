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
		HEAL,
		TELEPORT,

		MAX,
	};
}

class GameKeyBind : public Manager
{
private:
	struct Info
	{
		KeyType::Kind type;
		INT8 nKey;
		std::shared_ptr<DelegateBase<const bool> > pState;
		std::shared_ptr<DelegateBase<void> > pAction;
	};

public:
	GameKeyBind() {}
	~GameKeyBind() {}

	void Initialize();
	void Finalize();
	void Update();

	void SetKeyInfo(KeyBind::Kind bind, KeyType::Kind type, INT8 key, const std::shared_ptr<DelegateBase<void> >&  pAction, const std::shared_ptr<DelegateBase<const bool> > pState = nullptr);

private:
	std::vector<Info> m_Info;
};