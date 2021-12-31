#pragma once
#include <App/Level/LevelBase.h>

class LV_MasterWitch : public LevelBase
{
public:
	LV_MasterWitch() {}
	~LV_MasterWitch() override{}

	void Create(std::weak_ptr<SceneBase> pScene)override;

private:

};