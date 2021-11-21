#pragma once
#include<System/ClassDesign/Delegate.h>
#include <System/DebugLog.h>
#include<Defines.h>


template<class T>
class State
{
public:
	using PTR = std::unique_ptr<State<T> >;
	using PTRLIST = std::vector<PTR>;

private:
	using ACTION_DELEGATE = std::shared_ptr<DelegateBase<const bool> >;
	using TRANS_DELEGATE = std::shared_ptr<DelegateBase<const int> >;

	using ActionFunc = const bool(T::*)();
	using TransFunc = const int(T::*)();
public:
	State() {}
	~State() {}

	const bool Action()
	{
		if (m_pEventFuncList.empty())
		{
			DebugLog::GetInstance().FreeError("Stateの実行イベントが設定されていません");
			return true;
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

	const int Next()
	{
		if (PTRNULLCHECK(m_pTransFunc))
		{
			DebugLog::GetInstance().FreeError("Stateの遷移関数が設定されていません");
			return 0;
		}
		return m_pTransFunc->Execute();
	}
	
	/*
	* @brief メインイベントを登録
	* @param[pT] weakptr<T>のアドレス
	* @param[func] ActionFunc 戻り値const bool,引数なし型の関数を設定してね						
	*/
	void AddActionFunc(const std::weak_ptr<T>& pT, const ActionFunc func)
	{
		m_pEventFuncList.push_back(Delegate<T, const bool>::CreateDelegator(pT, func));
	}

	/*
	* @brief ステート遷移
	* @param[pT] weakptr<T>のアドレス
	* @param[func] TransFunc 戻り値const sint,引数なし型の関数を設定してね						
	*/
	void SetTransitionFunc(const std::weak_ptr<T>& pT, const TransFunc func)
	{
		m_pTransFunc = Delegate<T, const int>::CreateDelegator(pT, func);
	}

private:
	std::vector<ACTION_DELEGATE> m_pEventFuncList;
	TRANS_DELEGATE m_pTransFunc;
};