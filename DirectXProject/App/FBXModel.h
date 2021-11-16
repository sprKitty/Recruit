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
		DirectX::XMFLOAT4X4 matrix;		// 骨の姿勢
		DirectX::XMMATRIX preRotate;	// あらかじめ回転している量
		MatrixParam param;	 // デフォルトの骨の姿勢
	};
	struct AnimeInfo
	{
		float playFrame;	// 現在再生中のフレーム
		bool loop;		// 繰り返し再生するかどうか
		float speed;	// アニメーションの再生スピード

		ggfbx::AnimationInfo* pAnime;	// アニメの情報
	};
	struct FBXVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
		// メッシュとボーンを結び付けるためのデータ
		float weight[4];	// それぞれの骨からどの程度影響を受けるか
		long index[4];		// どの骨に引っ付いて動くか
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
	// 戻り値で配列のどの位置にアニメーションの情報を格納したか返す
	// 読み込めなかった時は-1を返す
	int LoadAnime(const char* fileName);

	// アニメーションの再生指定
	void PlayAnime(int animeNo, bool isLoop = false);
	// 遷移ｱﾆﾒｰｼｮﾝの指定
	void PlayBlendAnime(int blendNo, float time, bool isLoop = false);
	// アニメーション再生判定
	bool IsPlayAnime(int animeNo = -1);
	// ブレンドアニメーション再生判定
	bool IsPlayBlendAnime();

	// Update
	// 映像と同じイメージで早送り、コマ送りなどを考えたときに
	// １フレームでの処理数が一定ではないので、現在の再生スピードで
	// どれだけ処理するかで考えた→結果stepという関数
	void Step(float speed = 1.0f);
	void Draw(void(*PreCallBack)(int meshIndex) = nullptr);

	DirectX::XMFLOAT4X4 GetBone(int index);

	std::vector<MeshInverse>& GetMeshInverse() { return m_meshInverse; }
private:
	// 全てのメモリを安全に解放するための関数
	void Reset();
	// モデルの骨がどんなポーズを取っているか計算する関数
	void CalcBone(int index, DirectX::XMMATRIX parent);
	// 骨を描画する関数
	void DrawBone(int index, DirectX::XMFLOAT3 parent);
	// アニメーションの時間経過を計算
	void CalcAnimeFrame(int animeNo, float speed);
	// アニメーションのフレームに応じて骨のポーズを計算
	void CalcAnime(int animeNo, MatrixParam* pBones);
	// アニメーションのフレームに対応した姿勢を計算
	float CalcKey(ggfbx::AnimationInfo::CurveInfo::List& keys, float frame, float defValue);


private:
	// --- 骨の情報
	// 骨一本一本の位置や回転の情報、
	// どの骨がどこに繋がってるか、骨の名称が含まれる
	// また、どこから計算を始めるのかといった情報を含む
	ggfbx::BoneListInfo* m_pFBXBone;	// FBXの骨情報
	BoneInfo* m_pBones;	// プログラム実行中の骨の情報
	const static int MaxBlend = 2;
	MatrixParam* m_pBlendBones[MaxBlend];	// 遷移用の骨情報

	// --- アニメーションの情報
	const static int MaxAnime = 10;
	AnimeInfo* m_pAnimes[MaxAnime];
	int m_playAnimeNo;	// 現在再生中のアニメ番号

	int m_blendAnimeNo;	// 遷移中のアニメ番号
	float m_blendFrame;	// 遷移時間

	// モデルの表示
	DrawBuffer* m_pBuffers;
	int m_meshNum;
	std::vector<MeshInverse> m_meshInverse;

};