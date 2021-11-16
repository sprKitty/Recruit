#pragma once
#include <DirectXMath.h>
#include <Vector.h>
#include <math.h>
#include <Transform.h>

#define RAD(rot)rot* DirectX::XM_PI/180.0f
#define DEG(deg)deg*180.0f/DirectX::XM_PI
static const float RADMAX = 6.283185482f;

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
	* @brief DirectXFloat2を自作Vector2に変換
	* @return Vector2
	*************************************************/
	Vector2 XM2ConvertV2(DirectX::XMFLOAT2 xm2);
	
	/*************************************************
	* @brief 自作Vector2をDirectXFloat2に変換
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT2 V2ConvertXM2(Vector2 v2);
	
	/*************************************************
	* @brief DirectXFloat3を自作Vector3に変換
	* @return Vector3
	*************************************************/
	Vector3 XM3ConvertV3(DirectX::XMFLOAT3 xm3);
	
	/*************************************************
	* @brief 自作Vector4をDirectXFloat4に変換
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT3 V3ConvertXM3(Vector3 v3);
	
	/*************************************************
	* @brief DirectXFloat4を自作Vector4に変換
	* @return Vector4
	*************************************************/
	Vector4 XM4ConvertV4(DirectX::XMFLOAT4 xm4);
	
	/*************************************************
	* @brief 自作Vector2をDirectXFloat2に変換
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT4 V4ConvertXM4(Vector4 v4);

	/*************************************************
	* @brief 内積計算
	* @param Vector3 pos0 頂点座標0
	* @param Vector3 pos1 頂点座標1
	* @return Vector3 内積計算結果
	*************************************************/
	Vector3 Cross(Vector3* pPos0, Vector3* pPos1);
	
	/*************************************************
	* @brief 正規化計算
	* @param Vector3 pos0 頂点座標0
	* @param Vector3 pos1 頂点座標1
	* @return Vector3 正規化計算結果
	*************************************************/
	Vector3 Normalize(Vector3 v);

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
	* @brief 長さ計算
	* @param Vector3 dir 方向
	* @return float 長さ計算結果
	*************************************************/
	float Length(Vector3* pDir);
	
	/*************************************************
	* @brief 外積計算
	* @param Vector3 pos0 頂点座標0
	* @param Vector3 pos1 頂点座標1
	* @return float 外積計算結果
	*************************************************/
	float Dot(Vector3* pPos0, Vector3* pPos1);
	
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
}