#pragma once
#include <memory>

template<typename X, typename... Y>
class DelegateBase
{
public:
	DelegateBase() {}
	virtual ~DelegateBase() {}

	/*
	* @brief 設定されている関数を実行する
	*/
	virtual X Execute(Y...) = 0;

private:

};

/*
* @brief 
* @template T クラス
* @template X 戻り値
* @template Y 引数
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
	* @brief 関数を設定する
	* @param pObj 設定するクラスのウィークポインタ
	* @param func templateの引数XとYに該当する関数を設定する
	* @return 作成したデリゲート関数
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
	* @param pObj 設定するクラスのウィークポインタ
	* @param func templateの引数XとYに該当する関数を設定する
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