#pragma once
#include <App/Component/Magic/MagicBase.h>
#include<System/ClassDesign/StateBase.h>


class MagicBullet;

class MagicBall : public MagicBase
{
public:
	MagicBall() {}
	~MagicBall()override {}

	void Init()override;
	void Reset()override;
	void Uninit()override;
	void Update()override;

	inline void SetShotNum(const int num) { m_shot.nMaxNum = num; }

	/*
	* @brief ’e‚ğ”­Ë‚·‚éŠÔ‚ğİ’è‚·‚é
	* @param const float[fTime] ”­ËŠÔ
	* @param const float[fFluctuation] ”­ËŠÔ‚Ì+-ƒ‰ƒ“ƒ_ƒ€‚Å‚¸‚ç‚·
	*/
	void SetShotData(const float fTime, const float fFluctuation);

	void SetMoveShotPos(const Vector3& nowPos, const Vector3& shotPos);

public:
	static float MOVE_SPEED;	// ‰½•b‚Å–Ú“I’n‚É’…‚­‚©

private:
	struct Shot
	{
		int nNum;
		int nMaxNum;
		float fTime;
		float fMaxTime;
		float fDefault;
		float fFluctuation;
	};

	struct Move
	{
		float fTime;
		Vector3 vStart;
		Vector3 vEnd;
	};

private:
	void NextShot();
	const bool MoveShotPos();
	const bool PlayShot();

private:
	State<MagicBall>::PTR m_pState;
	std::vector<std::weak_ptr<MagicBullet> > m_pMagicBulletList;
	Shot m_shot;
	Move m_move;
};