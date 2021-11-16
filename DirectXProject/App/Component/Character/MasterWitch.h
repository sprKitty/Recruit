#pragma once
#include <App/Component/Component.h>


class MasterWitch : public Component
{
public:
	MasterWitch() {}
	~MasterWitch()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

private:

};