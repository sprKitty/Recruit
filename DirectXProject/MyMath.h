#pragma once
#include <DirectXMath.h>
#include <Vector.h>
#include <math.h>

static constexpr float RADMAX = 6.283185482f;

namespace MyMath
{
	struct Vertex
	{
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
		Vector3 tangent;
		Vector3 binormal;
	};
	/*************************************************
	* @brief ローカル座標をワールド座標に変換
	* @param[Vector3] scale 大きさ
	* @param[Vector3] rot ラジアン角
	* @param[Vector3] pos ローカル座標
	*************************************************/
	DirectX::XMMATRIX ConvertMatrix(Vector3 scale, Vector3 rot, Vector3 pos);

	/*************************************************
	* @brief 接空間、従法線計算
	* @param const Vertex& v0 頂点0に関する情報
	* @param const Vertex& v1 頂点1に関する情報
	* @param const Vertex& v2 頂点2に関する情報
	* @param Vector3* pBinormal 接空間計算結果
	* @param Vector3* pTangent 従法線計算結果
	*************************************************/
	bool TangentandBinormal(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vector3* pTangent, Vector3* pBinormal);
	
	/*************************************************
	* @brief ラジアン角計算
	* @param x0 座標
	* @param y0 座標
	* @param x1 座標
	* @param y1 座標
	* @return float ラジアン角計算結果
	*************************************************/
	float Radian(float x0, float y0, float x1, float y1);
	
	/*************************************************
	* @brief 長さ計算
	* @param float radius 半径
	* @param float angle 角度
	* @param float calc0 ラジアン角計算結果
	* @param float calc1 ラジアン角計算結果
	*************************************************/
	void RotRadiusCalcPoint(float radius, float angle, float& calc0, float& calc1);

	const float Lerp(const float fTime, const float fStart, const float fEnd);

	inline float GaussianDistribution(const Vector2& vPos, const float rho) { return exp(-(vPos.x * vPos.x + vPos.y * vPos.y) / (2.0f * rho * rho)); }

	inline const float EaseOutCubic(const float fTime) { return  1.0f - static_cast<float>(pow(1.0f - fTime, 3)); }
	
	inline const float EaseOutExpo(const float fTime) { return  1.0f - static_cast<float>(pow(2, -10 * fTime)); }

	inline const float EaseInOutCubic(const float fTime) { return fTime < 0.5f ? 4.0f * fTime * fTime * fTime : 1.0f - static_cast<float>(pow(-2 * fTime + 2, 3)) / 2.0f; }
}