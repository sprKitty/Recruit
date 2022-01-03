#include "SceneBase.h"
#include <App/FactoryMethod.h>

void SceneBase::AddObject(const Object::OWNER_OBJ & pObj)
{
	if (!PTRNULLCHECK(pObj))
	{
		m_pObjList.emplace_back(std::move(pObj));
	}
}

void SceneBase::MoveObject_FactoytoScene()
{
	FactoryMethod::GetInstance().MoveObjectList(m_pObjList);
}

void SceneBase::DeleteObject()
{
	for (auto itr = m_pObjList.begin(); itr != m_pObjList.end();)
	{
		if ((*itr)->IsDelete())
		{
			itr->reset();
			itr = m_pObjList.erase(itr);
		}
		if (itr == m_pObjList.end())
		{
			break;
		}
		++itr;
	}
}

const Object::WORKER_OBJECTLIST SceneBase::GetObjectList()
{
	return Object::ConvertWorker(m_pObjList);
}
