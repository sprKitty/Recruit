#pragma once
#include <DirectXMath.h>
#include <Vector.h>
#include <math.h>

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
	* @brief ���[�J�����W�����[���h���W�ɕϊ�
	* @param[Vector3] scale �傫��
	* @param[Vector3] rot ���W�A���p
	* @param[Vector3] pos ���[�J�����W
	*************************************************/
	DirectX::XMMATRIX ConvertMatrix(Vector3 scale, Vector3 rot, Vector3 pos);

	/*************************************************
	* @brief �ڋ�ԁA�]�@���v�Z
	* @param const Vertex& v0 ���_0�Ɋւ�����
	* @param const Vertex& v1 ���_1�Ɋւ�����
	* @param const Vertex& v2 ���_2�Ɋւ�����
	* @param Vector3* pBinormal �ڋ�Ԍv�Z����
	* @param Vector3* pTangent �]�@���v�Z����
	*************************************************/
	bool TangentandBinormal(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vector3* pTangent, Vector3* pBinormal);
	
	/*************************************************
	* @brief ���W�A���p�v�Z
	* @param x0 ���W
	* @param y0 ���W
	* @param x1 ���W
	* @param y1 ���W
	* @return float ���W�A���p�v�Z����
	*************************************************/
	float Radian(float x0, float y0, float x1, float y1);
	
	/*************************************************
	* @brief �����v�Z
	* @param float radius ���a
	* @param float angle �p�x
	* @param float calc0 ���W�A���p�v�Z����
	* @param float calc1 ���W�A���p�v�Z����
	*************************************************/
	void RotRadiusCalcPoint(float radius, float angle, float& calc0, float& calc1);
}