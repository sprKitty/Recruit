#pragma once
#include <App/Component/Component.h>
#include <App/Component/Object.h>
#include <MyMath.h>

class RootMotion : public Component
{
private:
	struct Info
	{
		Vector3 vPoint;
		VectorInt4 vNext;
		bool isUsed;
	};

public:
	RootMotion() {}
	~RootMotion()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	
	/*
	* @brief ルートを計算
	*/
	void CalcRoot(const Vector3& vDestination, const Vector3 vNowPos, bool isReset);

	/*
	* @brief ルート座標を追加 
	* @param[vPoint] Vector3 ルート座標
	* @param[vNext] VectorInt4 隣り合うポイントの添え字
	* @note vNext.x : 右
	* @note vNext.y : 左
	* @note vNext.z : 奥
	* @note vNext.w : 手前
	*/
	void AddPoint(const Vector3& vPoint, const VectorInt4& vNext);

	const Vector3& CalcGuide(const Vector3& vNowPos);


	inline const bool IsRootListEmpty()const { return m_vRootList.empty(); }
	inline void ClearRootList() { m_vRootList.clear(); }

private:
	void MinPointArray(std::vector<Info>* vPointList, const Vector3& vNowPos);

private:
	std::vector<Info> m_pointInfoList;
	std::vector<Info> m_calcPointInfoList;
	std::vector<Vector3> m_vRootList;
};