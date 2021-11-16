#pragma once
#include "Component.h"


class Player : public Component
{
public:
	Player();
	~Player();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
};