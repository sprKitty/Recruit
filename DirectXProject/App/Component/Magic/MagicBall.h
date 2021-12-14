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
	* @brief �e�𔭎˂��鎞�Ԃ�ݒ肷��
	* @param const float[fTime] ���ˎ���
	* @param const float[fFluctuation] ���ˎ��Ԃ�+-�����_���ł��炷
	*/
	void SetShotData(const float fTime, const float fFluctuation);

	void SetMoveShotPos(const Vector3& nowPos, const Vector3& shotPos);

public:
	static float MOVE_SPEED;	// ���b�ŖړI�n�ɒ�����

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