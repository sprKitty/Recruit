//#include "Delegate.h"

//template<typename T, typename X>
//class DelegateBase
//{
//public:
//	DelegateBase() {}
//	virtual ~DelegateBase() {}
//
//	virtual T operator()(X) = 0;
//
//private:
//
//};
//
//template<class T, typename X ,typename Y>
//class Delegate : public DelegateBase<X, Y>
//{
//public:
//	Delegate() {}
//	~Delegate() {}
//
//	typedef X (T::*EventFunc)(Y);
//
//	static DelegateBase<X, Y>* CreateDelegator(std::weak_ptr<T> pObj, EventFunc Event)
//	{
//		Delegate* dg = new Delegate();
//		dg->Set(pObj, Event);
//		return dg;
//	}
//
//	X operator()(Y value)override
//	{
//		return (m_pObj.lock().get()->*m_func)(value);
//	}
//
//
//	void Set(std::weak_ptr<T> pObj, EventFunc func)
//	{
//		m_pObj = pObj;
//		m_func = func;
//	}
//
//protected:
//	std::weak_ptr<T> m_pObj;
//	EventFunc m_func;
//private:
//
//};





//#include <Transform.h>
//#include <vector>
//#include <memory>
//#include <MyMath.h>
//#include <Defines.h>
//
//class Component;
//
//class Object
//{
//public:
//	Object();
//	~Object();
//
//
//	void Init();
//	void Uninit();
//	void Update();
//	void Draw();
//
//	//オブジェクトが持つコンポーネントを取得
//	template<class T>
//	std::weak_ptr<T> GetComponent()
//	{
//		std::weak_ptr<T> buff;
//		for (auto com : ComponentList)
//		{
//			buff = dynamic_cast<std::weak_ptr<T> >(com);
//			if (!PTRNULLCHECK(buff)) return buff;
//		}
//		return buff;
//	}
//
//	//オブジェクトが持つコンポーネントを追加
//	template<class T>
//	std::weak_ptr<T> AddComponent()
//	{
//		std::shared_ptr<T> buff(new T());
//		buff->m_pOwner = this;
//		ComponentList.push_back(buff);
//		buff->Init();
//		return buff;
//	}
//
//	inline const Transform& GetTransform()
//	{
//		return m_Transform;
//	}
//
//	inline void SetTransform(const Transform& transform)
//	{
//		m_Transform = transform;
//	}
//
//	inline const XMMATRIX& GetWorldMatrix()
//	{
//		return MyMath::ConvertMatrix(m_Transform.scale, m_Transform.rot, m_Transform.pos);
//	}
//
//	std::weak_ptr<Object> GetParent()const
//	{
//		return m_pParent;
//	}
//
//	void Parent(const std::weak_ptr<Object> pObj)
//	{
//		m_pParent = pObj;
//	}
//
//private:
//	std::vector<std::shared_ptr<Component> > ComponentList;
//	Transform m_Transform;
//	std::weak_ptr<Object> m_pParent;
//};