#pragma once
//=================================
// class：Object
//---------------------------------
// 作成者：矢橋嵩琉
// コンポーネントを管理するクラス
//---------------------------------
// 制作日：2021/03/13～15
//
//=================================
#include <Defines.h>
#include <iostream>

class Component;

class Object : public std::enable_shared_from_this<Object>
{
public:
	enum class Type
	{
		NONE,
		PLAYER,
		BOSS_WITCH,
		LEVEL,

		MAX
	};

	using OWNER_OBJECTLIST = std::vector<std::shared_ptr<Object> >;
	using WORKER_OBJECTLIST = std::vector<std::weak_ptr<Object> >;
	using OWNER_OBJ = std::shared_ptr<Object>;
	using WORKER_OBJ = std::weak_ptr<Object>;

	inline static WORKER_OBJECTLIST ConvertWorker(OWNER_OBJECTLIST& pObjects)
	{
		WORKER_OBJECTLIST pWorkerObjects;

		for (auto itr : pObjects)
		{
			WORKER_OBJ pObj(itr);

			pWorkerObjects.push_back(itr);
		}

		return pWorkerObjects;
	}

public:
	Object();
	~Object();


	void Init();
	void Uninit();
	void Update();

	//オブジェクトが持つコンポーネントを取得
	template<class T>
	inline std::weak_ptr<T> GetComponent()
	{
		std::weak_ptr<T> buff;
		for (std::shared_ptr<Component> com : ComponentList)
		{
			buff = std::dynamic_pointer_cast<T>(com);
			if (!PTRNULLCHECK(buff)) return buff;
		}
//#ifdef _DEBUG
		//std::cout << buff << "クラスが見つかりませんでした" << std::endl;
//#endif // _DEBUG

		return buff;
	}

	//オブジェクトが持つコンポーネントを追加
	template<class T>
	inline std::weak_ptr<T> AddComponent()
	{
		std::shared_ptr<T> buff(new T());
		buff->m_pOwner = weak_from_this();
		ComponentList.push_back(buff);
		buff->Init();
		return buff;
	}

	template<class T>
	inline void RemoveCompont(std::weak_ptr<Component> pCom)
	{
		for (std::vector<std::shared_ptr<Component> >::iterator itr = ComponentList.begin(); itr != ComponentList.end();)
		{
			if (pCom.lock().get() == itr->get()) // アドレスが同じオブジェクトか
			{
				T* buff = dynamic_cast<T*>(itr->get());
				buff->Uninit();
				itr = ComponentList.erase(itr);
				return;
			}
			if (itr == ComponentList.end())break;
			++itr;
		}
	}

	inline const std::weak_ptr<Object>& GetParent()const
	{
		return m_pParent;
	}

	inline void Parent(const std::weak_ptr<Object>& pObj)
	{
		m_pParent = pObj;
	}

	inline const Type& GetType()
	{ 
		return m_type; 
	}

	inline void SetType(const Type& type) 
	{ 
		m_type = type;
	}

private:
	std::vector<std::shared_ptr<Component> > ComponentList;
	Type m_type;
	std::weak_ptr<Object> m_pParent;
};