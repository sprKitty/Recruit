#pragma once
#include "LevelBase.h"

class LV_Cow final : public LevelBase
{
public:
	LV_Cow() {}
	virtual ~LV_Cow() override {}

	virtual void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)override;
	virtual const Level_Type::Kind Transition(const Object::WORKER_OBJ pObject)override;
	virtual void Finalize(const Object::WORKER_OBJ pObject)override;

private:

};