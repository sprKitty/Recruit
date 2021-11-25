#pragma once
#include <App/Component/Magic/MagicBase.h>



class MagicBullet : public MagicBase
{
public:
	MagicBullet() {}
	~MagicBullet()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void SetStartPos(const Vector3& vPos);
	
	void SetDiretion(const Vector3& vDir)
	{
		m_vMoveDirection = vDir;
	}

private:
	static float MOVE_SPEED;
	static float MAX_SURVIVETIME;

	Vector3 m_vMoveDirection;
	float m_fSurviveTime;
};
