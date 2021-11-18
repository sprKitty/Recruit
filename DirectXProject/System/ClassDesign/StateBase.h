#pragma once
#include<System/ClassDesign/Delegate.h>
#include <System/DebugLog.h>
#include<Defines.h>


template<class T>
class State
{
private:
	using ACTION_DELEGATE = std::shared_ptr<DelegateBase_void<const bool> >;
	using TRANS_DELEGATE = std::shared_ptr<DelegateBase_void<const int> >;

	using ActionFunc = const bool(T::*)();
	using TransFunc = const int(T::*)();
public:
	State() {}
	virtual ~State() {}

	virtual const bool Action()
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

	void AddActionFunc(const std::weak_ptr<T>& pT, const ActionFunc func)
	{
		m_pEventFuncList.push_back(Delegate_void<T, const bool>::CreateDelegator(pT, func));
	}

	void SetTransitionFunc(const std::weak_ptr<T>& pT, const TransFunc func)
	{
		m_pTransFunc = Delegate_void<T, const int>::CreateDelegator(pT, func);
	}

private:
	std::vector<ACTION_DELEGATE> m_pEventFuncList;
	TRANS_DELEGATE m_pTransFunc;
};