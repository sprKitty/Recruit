#include "GameKeyBind.h"
#include <System/DebugLog.h>

void GameKeyBind::Initialize()
{
	m_Info.resize(KeyBind::MAX);

	for (int i = 0; i < KeyBind::MAX; ++i)
	{
		m_Info[i].nKey = -1;
		m_Info[i].type = KeyType::MAX;
	}
}

void GameKeyBind::Finalize()
{
}

void GameKeyBind::Update()
{
	for (const auto& itr : m_Info)
	{
		if (itr.nKey == -1
			|| itr.type == KeyType::MAX
			|| itr.pAction.use_count() == 0)
		{
			DebugLog::GetInstance().FreeError("ƒL[‚ªÝ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB");
			continue;
		}
		else
		{
			bool isInput = false;
			switch (itr.type)
			{
			case KeyType::TRIGGER:
				isInput = IsKeyTrigger(itr.nKey);
				break;
			case KeyType::PRESS:
				isInput = IsKeyPress(itr.nKey);
				break;
			case KeyType::RELEASE:
				isInput = IsKeyRelease(itr.nKey);
				break;
			case KeyType::REPEAT:
				isInput = IsKeyRepeat(itr.nKey);
				break;
			case KeyType::MAX:
			default:
				break;
			}
			if (isInput)
			{
				if (itr.pState.use_count() > 0)
				{
					isInput = itr.pState->Execute();
				}
				else
				{
					isInput = !isInput;
				}

				if (isInput)
				{
					itr.pAction->Execute();
				}
			}
		}
	}
}

void GameKeyBind::SetKeyInfo(KeyBind::Kind bind, KeyType::Kind type, INT8 key, const std::shared_ptr<DelegateBase<void>>& pAction, const std::shared_ptr<DelegateBase<const bool>> pState)
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
