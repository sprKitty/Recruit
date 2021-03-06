#pragma once
//=================================
// class：Object
//---------------------------------
// 作成者：矢橋嵩琉
// コンポーネントを管理するクラス
//---------------------------------
// 制作日：2021/03/13〜15
//
//=================================
#include <Defines.h>
#include <iostream>

namespace ObjectType
{
	enum Kind
	{
		NONE,				
		PLAYER,				// プレイヤー
		PLAYERATTACK,		// プレイヤーの攻撃
		BOSSWITCH,			// 魔女
		BOSSATTACK1,		// 魔女の攻撃1
		BOSSATTACK2,		// 魔女の攻撃1
		STAGE,				// ステージ
		OUTSIDE,			// エリア外
		OUTSIDE_NORTH,		// エリア外北
		OUTSIDE_EAST,		// エリア外東
		OUTSIDE_WEST,		// エリア外西
		OUTSIDE_SOUTH,		// エリア外南

		MAX
	};
}

class Component;

class Object : public std::enable_shared_from_this<Object>
{
public:
	using OWNER_OBJ = std::shared_ptr<Object>;
	using WORKER_OBJ = std::weak_ptr<Object>;
	using OWNER_OBJECTLIST = std::vector<OWNER_OBJ>;
	using WORKER_OBJECTLIST = std::vector<WORKER_OBJ>;

	inline static WORKER_OBJECTLIST ConvertWorker(OWNER_OBJECTLIST& pObjects)
	{
		WORKER_OBJECTLIST pWorkerObjects;

		for (auto itr : pObjects)
		{
			WORKER_OBJ pObj(itr);

			pWorkerObjects.emplace_back(itr);
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
	inline const std::weak_ptr<T> GetComponent()
	{
		std::weak_ptr<T> buff;
		for (std::shared_ptr<Component> com : m_pComponentList)
		{
			buff = std::dynamic_pointer_cast<T>(com);
			if (!buff.expired()) return buff;
		}

		return buff;
	}

	template<class T>
	inline const std::vector<std::weak_ptr<T>> GetComponentList()
	{
		std::vector<std::weak_ptr<T>> pList;
		std::weak_ptr<T> buff;
		for (std::shared_ptr<Component> itr : m_pComponentList)
		{
			buff = std::dynamic_pointer_cast<T>(itr);
			if (!buff.expired()) pList.emplace_back(buff); // 空でなければ追加
		}
		return pList;
	}

	//オブジェクトが持つコンポーネントを追加
	template<class T>
	inline const std::weak_ptr<T> AddComponent()
	{
		std::shared_ptr<T> buff(new T());
		buff->m_pOwner = weak_from_this();
		m_pComponentList.emplace_back(buff);
		buff->Init();
		return buff;
	}

	template<class T>
	inline void RemoveComponent()
	{
		for (std::vector<std::shared_ptr<Component> >::iterator itr = m_pComponentList.begin(); itr != m_pComponentList.end();)
		{
			T* buff = dynamic_cast<T*>(itr->get());
			if (buff)
			{
				buff->Uninit();
				itr = m_pComponentList.erase(itr);
			}

			if (itr == m_pComponentList.end())break;
			++itr;
		}
	}

	void EndSortComponent(std::weak_ptr<Component> pCom)
	{
		std::vector<std::shared_ptr<Component> > list = std::move(m_pComponentList);
		int num = -1;
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i].get() == pCom.lock().get())
			{
				num = i;
			}
			else
			{
				m_pComponentList.emplace_back(std::move(list[i]));
			}
		}
		if (num != -1)
		{
			m_pComponentList.emplace_back(std::move(list[num]));
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

	inline const ObjectType::Kind GetType()
	{ 
		return m_type; 
	}

	inline void SetType(const ObjectType::Kind type)
	{ 
		m_type = type;
	}

	inline void EnableDelete()
	{
		m_isDelete = true;
	}

	inline const bool IsDelete()
	{
		return m_isDelete;
	}

	inline void EnableActive()
	{
		m_isActive = true;
	}

	inline void DisableActive()
	{
		m_isActive = false;
	}

	inline const bool IsActive()
	{
		return m_isActive;
	}


private:
	ObjectType::Kind m_type;
	std::weak_ptr<Object> m_pParent;
	std::vector<std::shared_ptr<Component> > m_pComponentList;
	bool m_isDelete;
	bool m_isActive;
};