#pragma once
#include <memory>

template<typename X, typename... Y>
class DelegateBase
{
public:
	DelegateBase() {}
	virtual ~DelegateBase() {}

	/*
	* @brief �ݒ肳��Ă���֐������s����
	*/
	virtual X Execute(Y...) = 0;

private:

};

/*
* @brief 
* @template T �N���X
* @template X �߂�l
* @template Y ����
*/
template<class T, typename X, typename... Y>
class Delegate final : public DelegateBase<X, Y...>
{
private:
	using EventFunc = X(T::*)(Y...);

public:
	Delegate() {}
	~Delegate() {}


	/*
	* @brief �֐���ݒ肷��
	* @param pObj �ݒ肷��N���X�̃E�B�[�N�|�C���^
	* @param func template�̈���X��Y�ɊY������֐���ݒ肷��
	* @return �쐬�����f���Q�[�g�֐�
	*/
	inline static std::shared_ptr<DelegateBase<X, Y...> > CreateDelegator(const std::weak_ptr<T>& pObj, const EventFunc func)
	{
		std::shared_ptr<Delegate> pDG(new Delegate());
		pDG->Set(pObj, func);
		return pDG;
	}

	
	inline X Execute(Y... value)override
	{
		return (m_pObj.lock().get()->*m_func)(value...);
	}

private:

	/*
	* @param pObj �ݒ肷��N���X�̃E�B�[�N�|�C���^
	* @param func template�̈���X��Y�ɊY������֐���ݒ肷��
	*/
	inline void Set(const std::weak_ptr<T>& pObj, const EventFunc func)
	{
		m_pObj = pObj;
		m_func = func;
	}

private:
	std::weak_ptr<T> m_pObj;
	EventFunc m_func;

};