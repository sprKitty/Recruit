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
			}
			if (isInput)
			{
				itr.pAction->Execute();
			}
		}
	}
}
