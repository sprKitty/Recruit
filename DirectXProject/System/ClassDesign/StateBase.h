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
			DebugLog::GetInstance().FreeError("State�̎��s�C�x���g���ݒ肳��Ă��܂���");
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
			DebugLog::GetInstance().FreeError("State�̑J�ڊ֐����ݒ肳��Ă��܂���");
			return 0;
		}
		return m_pTransFunc->Execute();
	}
	
	/*
	* @brief ���C���C�x���g��o�^
	* @param[pT] weakptr<T>�̃A�h���X
	* @param[func] ActionFunc �߂�lconst bool,�����Ȃ��^�̊֐���ݒ肵�Ă�						
	*/
	void AddActionFunc(const std::weak_ptr<T>& pT, const ActionFunc func)
	{
		m_pEventFuncList.push_back(Delegate<T, const bool>::CreateDelegator(pT, func));
	}

	/*
	* @brief �X�e�[�g�J��
	* @param[pT] weakptr<T>�̃A�h���X
	* @param[func] TransFunc �߂�lconst sint,�����Ȃ��^�̊֐���ݒ肵�Ă�						
	*/
	void SetTransitionFunc(const std::weak_ptr<T>& pT, const TransFunc func)
	{
		m_pTransFunc = Delegate<T, const int>::CreateDelegator(pT, func);
	}

private:
	std::vector<ACTION_DELEGATE> m_pEventFuncList;
	TRANS_DELEGATE m_pTransFunc;
};