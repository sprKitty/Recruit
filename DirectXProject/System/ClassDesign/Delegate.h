#pragma once
#include <memory>

template<typename X, typename... Y>
class DelegateBase
{
public:
	DelegateBase() {}
	virtual ~DelegateBase() {}

	virtual X Execute(Y...) = 0;

private:

};

/*
* @brief 
* @template T ÉNÉâÉX
* @template X ñﬂÇËíl
* @template Y à¯êî
*/
template<class T, typename X, typename... Y>
class Delegate : public DelegateBase<X, Y...>
{
public:
	Delegate() {}
	~Delegate() {}

	typedef X(T::*EventFunc)(Y...);

	static std::shared_ptr<DelegateBase<X, Y...> > CreateDelegator(const std::weak_ptr<T>& pObj, const EventFunc Event)
	{
		std::shared_ptr<Delegate> pDG(new Delegate());
		pDG->Set(pObj, Event);
		return pDG;
	}


	X Execute(Y... value)override
	{
		return (m_pObj.lock().get()->*m_func)(value...);
	}

	void Set(const std::weak_ptr<T>& pObj, const EventFunc func)
	{
		m_pObj = pObj;
		m_func = func;
	}

private:
	std::weak_ptr<T> m_pObj;
	EventFunc m_func;

};