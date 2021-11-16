#pragma once
#include "Component.h"

class Timer;

class Water : public Component
{
public:
	Water();
	~Water();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	Timer* m_pWaveSpeed;
};