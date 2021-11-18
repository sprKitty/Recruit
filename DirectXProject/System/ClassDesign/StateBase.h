#pragma once
#include<System/ClassDesign/Delegate.h>
#include <System/DebugLog.h>
#include<Defines.h>


class StateBase
{
protected:
	using EVENT_FUNC = std::shared_ptr<DelegateBase_void<const bool> >;
	using TRANS_FUNC = std::shared_ptr<DelegateBase_void<const int> >;

public:
	StateBase() {}
	virtual ~StateBase() {}

	virtual const bool Execute()
	{
		if (m_pEventFuncList.empty())
		{
			DebugLog::GetInstance().FreeError("Stateの実行イベントが設定されていません");
			return false;
		}
		for (const auto& itr : m_pEventFuncList)
		{
			if (!itr->Execute())
			{
				return false;
			}
		}
		return true;
	}

	virtual const int Next()
	{
		if (PTRNULLCHECK(m_pTransFunc))
		{
			DebugLog::GetInstance().FreeError("Stateの遷移関数が設定されていません");
			return 0;
		}
		return m_pTransFunc->Execute();
	}

	void AddEventFunc(const EVENT_FUNC& pEvent)
	{
		m_pEventFuncList.push_back(pEvent);
	}

	void SetTransitionFunc(const TRANS_FUNC& pTrans)
	{
		m_pTransFunc = pTrans;
	}

private:
	std::vector<EVENT_FUNC> m_pEventFuncList;
	TRANS_FUNC m_pTransFunc;
};