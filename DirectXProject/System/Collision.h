#pragma once
#include <App/Component/Object.h>

#define COLLISION Collision::GetInstance()

class Collider;

class Collision
{
public:
	static Collision* GetInstance()
	{
		static Collision instance;
		return &instance;
	}

	void Uninit();
	void Update();
	void Draw();
	void Delete(Collider* pCollider = nullptr);

	std::list<Collider*> m_pColliderList;

private:
	Collision() {}
	~Collision() {}

	//bool RaySegment(Collider *pA, Collider *pB);
	bool OBBvsOBB(Collider *pA, Collider *pB);
	/*
	@ƒoƒO‚ ‚è
	*/
	bool OBB(Collider *pA, Collider *pB);
	float LenSegOnSeparateAxis(Vector3 *pSep, Vector3* pE1, Vector3* pE2, Vector3* pE3 = 0);

};