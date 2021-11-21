#pragma once
#include "Component.h"


namespace Collision_Type
{

};

class Collider : public Component
{
public:
	Collider() {}
	~Collider()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

private:
};