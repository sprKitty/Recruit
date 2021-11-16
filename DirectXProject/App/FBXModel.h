#pragma once
#include <FBX/FBXLoader.h>
#include <System/DrawBuffer.h>
#include <MyMath.h>

class FBXPlayer
{
public:
	struct MatrixParam
	{
		DirectX::XMFLOAT3 translate;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;
	};
	struct BoneInfo
	{
		DirectX::XMFLOAT4X4 matrix;		// ���̎p��
		DirectX::XMMATRIX preRotate;	// ���炩���߉�]���Ă����
		MatrixParam param;	 // �f�t�H���g�̍��̎p��
	};
	struct AnimeInfo
	{
		float playFrame;	// ���ݍĐ����̃t���[��
		bool loop;		// �J��Ԃ��Đ����邩�ǂ���
		float speed;	// �A�j���[�V�����̍Đ��X�s�[�h

		ggfbx::AnimationInfo* pAnime;	// �A�j���̏��
	};
	struct FBXVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
		// ���b�V���ƃ{�[�������ѕt���邽�߂̃f�[�^
		float weight[4];	// ���ꂼ��̍�����ǂ̒��x�e�����󂯂邩
		long index[4];		// �ǂ̍��Ɉ����t���ē�����
	};
	struct MeshInverse
	{
		struct List
		{
			int boneIndex;
			DirectX::XMMATRIX invMat;
		};
		List* pList;
		int num;
	};

public:
	FBXPlayer();
	~FBXPlayer();

	bool Load(const char* fileName);
	// �߂�l�Ŕz��̂ǂ̈ʒu�ɃA�j���[�V�����̏����i�[�������Ԃ�
	// �ǂݍ��߂Ȃ���������-1��Ԃ�
	int LoadAnime(const char* fileName);

	// �A�j���[�V�����̍Đ��w��
	void PlayAnime(int animeNo, bool isLoop = false);
	// �J�ڱ�Ұ��݂̎w��
	void PlayBlendAnime(int blendNo, float time, bool isLoop = false);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnime(int animeNo = -1);
	// �u�����h�A�j���[�V�����Đ�����
	bool IsPlayBlendAnime();

	// Update
	// �f���Ɠ����C���[�W�ő�����A�R�}����Ȃǂ��l�����Ƃ���
	// �P�t���[���ł̏����������ł͂Ȃ��̂ŁA���݂̍Đ��X�s�[�h��
	// �ǂꂾ���������邩�ōl����������step�Ƃ����֐�
	void Step(float speed = 1.0f);
	void Draw(void(*PreCallBack)(int meshIndex) = nullptr);

	DirectX::XMFLOAT4X4 GetBone(int index);

	std::vector<MeshInverse>& GetMeshInverse() { return m_meshInverse; }
private:
	// �S�Ẵ����������S�ɉ�����邽�߂̊֐�
	void Reset();
	// ���f���̍����ǂ�ȃ|�[�Y������Ă��邩�v�Z����֐�
	void CalcBone(int index, DirectX::XMMATRIX parent);
	// ����`�悷��֐�
	void DrawBone(int index, DirectX::XMFLOAT3 parent);
	// �A�j���[�V�����̎��Ԍo�߂��v�Z
	void CalcAnimeFrame(int animeNo, float speed);
	// �A�j���[�V�����̃t���[���ɉ����č��̃|�[�Y���v�Z
	void CalcAnime(int animeNo, MatrixParam* pBones);
	// �A�j���[�V�����̃t���[���ɑΉ������p�����v�Z
	float CalcKey(ggfbx::AnimationInfo::CurveInfo::List& keys, float frame, float defValue);


private:
	// --- ���̏��
	// ����{��{�̈ʒu���]�̏��A
	// �ǂ̍����ǂ��Ɍq�����Ă邩�A���̖��̂��܂܂��
	// �܂��A�ǂ�����v�Z���n�߂�̂��Ƃ����������܂�
	ggfbx::BoneListInfo* m_pFBXBone;	// FBX�̍����
	BoneInfo* m_pBones;	// �v���O�������s���̍��̏��
	const static int MaxBlend = 2;
	MatrixParam* m_pBlendBones[MaxBlend];	// �J�ڗp�̍����

	// --- �A�j���[�V�����̏��
	const static int MaxAnime = 10;
	AnimeInfo* m_pAnimes[MaxAnime];
	int m_playAnimeNo;	// ���ݍĐ����̃A�j���ԍ�

	int m_blendAnimeNo;	// �J�ڒ��̃A�j���ԍ�
	float m_blendFrame;	// �J�ڎ���

	// ���f���̕\��
	DrawBuffer* m_pBuffers;
	int m_meshNum;
	std::vector<MeshInverse> m_meshInverse;

};