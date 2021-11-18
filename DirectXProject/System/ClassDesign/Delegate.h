#pragma once
#include <memory>

template<typename X, typename Y>
class DelegateBase
{
public:
	DelegateBase() {}
	virtual ~DelegateBase() {}

	virtual X Execute(Y) = 0;

private:

};

/*
* @brief 
* @template T ÉNÉâÉX
* @template X ñﬂÇËíl
* @template Y à¯êî
*/
template<class T, typename X, typename Y>
class Delegate : public DelegateBase<X, Y>
{
public:
	Delegate() {}
	~Delegate() {}

	typedef X(T::*EventFunc)(Y);

	static std::shared_ptr<DelegateBase<X, Y> > CreateDelegator(const std::weak_ptr<T>& pObj, const EventFunc Event)
	{
		std::shared_ptr<Delegate> pDG(new Delegate());
		pDG->Set(pObj, Event);
		return pDG;
	}


	X Execute(Y value)override
	{
		return (m_pObj.lock().get()->*m_func)(value);
	}

	void Set(const std::weak_ptr<T>& pObj, const EventFunc func)
	{
		m_pObj = pObj;
		m_func = func;
	}

protected:
	std::weak_ptr<T> m_pObj;
	EventFunc m_func;
private:

};


template<typename X>
class DelegateBase_void
{
public:
	DelegateBase_void() {}
	virtual ~DelegateBase_void() {}

	virtual X Execute() = 0;

private:

};

template<class T, typename X>
class Delegate_void : public DelegateBase_void<X>
{
public:
	Delegate_void() {}
	~Delegate_void() {}

	using EventFunc = X(T::*)();

	static std::shared_ptr<DelegateBase_void<X> > CreateDelegator(const std::weak_ptr<T>& pObj, const EventFunc Event)
	{
		std::shared_ptr<Delegate_void> pDG(new Delegate_void());
		pDG->Set(pObj, Event);
		return pDG;
	}


	X Execute()override
	{
		return (m_pObj.lock().get()->*m_func)();
	}

	void Set(const std::weak_ptr<T>& pObj, const EventFunc func)
	{
		m_pObj = pObj;
		m_func = func;
	}

protected:
	std::weak_ptr<T> m_pObj;
	EventFunc m_func;
private:

};