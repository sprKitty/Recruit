#include "LV_Cow.h"
#include <App/FactoryMethod.h>

void LV_Cow::Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)
{
	if (pScene.expired())return;
	if (pObject.expired())return;
}

const Level_Type::Kind LV_Cow::Transition(const Object::WORKER_OBJ pObject)
{
	if (!pObject.expired())
	{
		std::weak_ptr<Collider> pCollider = pObject.lock()->GetComponent<Collider>();
		if (!pCollider.expired())
		{
			Collider::HitInfo info = pCollider.lock()->GetHitInfo(CollisionType::AABB);
			if (info.isFlg)
			{
				if (!info.pObj.expired())
				{
					ObjectType::Kind type = info.pObj.lock()->GetType();

					switch (type)
					{
					case ObjectType::OUTSIDE_WEST:
						return Level_Type::BRANCH;

					case ObjectType::NONE:
					case ObjectType::PLAYER:
					case ObjectType::PLAYERATTACK:
					case ObjectType::BOSSWITCH:
					case ObjectType::BOSSATTACK1:
					case ObjectType::BOSSATTACK2:
					case ObjectType::STAGE:
					case ObjectType::OUTSIDE:
					case ObjectType::OUTSIDE_NORTH:
					case ObjectType::OUTSIDE_EAST:
					case ObjectType::OUTSIDE_SOUTH:
					case ObjectType::MAX:
					default:
						break;
					}
				}
			}
		}
	}
	return Level_Type::BRANCH;
}

void LV_Cow::Finalize(const Object::WORKER_OBJ pObject)
{
}