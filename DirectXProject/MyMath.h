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
	* @brief ���[�J�����W�����[���h���W�ɕϊ�
	* @param[Vector3] scale �傫��
	* @param[Vector3] rot ���W�A���p
	* @param[Vector3] pos ���[�J�����W
	*************************************************/
	DirectX::XMMATRIX ConvertMatrix(Vector3 scale, Vector3 rot, Vector3 pos);
	
	/*************************************************
	* @brief DirectXFloat2������Vector2�ɕϊ�
	* @return Vector2
	*************************************************/
	Vector2 XM2ConvertV2(DirectX::XMFLOAT2 xm2);
	
	/*************************************************
	* @brief ����Vector2��DirectXFloat2�ɕϊ�
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT2 V2ConvertXM2(Vector2 v2);
	
	/*************************************************
	* @brief DirectXFloat3������Vector3�ɕϊ�
	* @return Vector3
	*************************************************/
	Vector3 XM3ConvertV3(DirectX::XMFLOAT3 xm3);
	
	/*************************************************
	* @brief ����Vector4��DirectXFloat4�ɕϊ�
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT3 V3ConvertXM3(Vector3 v3);
	
	/*************************************************
	* @brief DirectXFloat4������Vector4�ɕϊ�
	* @return Vector4
	*************************************************/
	Vector4 XM4ConvertV4(DirectX::XMFLOAT4 xm4);
	
	/*************************************************
	* @brief ����Vector2��DirectXFloat2�ɕϊ�
	* @return XMFLOAT4
	*************************************************/
	DirectX::XMFLOAT4 V4ConvertXM4(Vector4 v4);

	/*************************************************
	* @brief ���όv�Z
	* @param Vector3 pos0 ���_���W0
	* @param Vector3 pos1 ���_���W1
	* @return Vector3 ���όv�Z����
	*************************************************/
	Vector3 Cross(Vector3* pPos0, Vector3* pPos1);
	
	/*************************************************
	* @brief ���K���v�Z
	* @param Vector3 pos0 ���_���W0
	* @param Vector3 pos1 ���_���W1
	* @return Vector3 ���K���v�Z����
	*************************************************/
	Vector3 Normalize(Vector3 v);

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
	* @brief �����v�Z
	* @param Vector3 dir ����
	* @return float �����v�Z����
	*************************************************/
	float Length(Vector3* pDir);
	
	/*************************************************
	* @brief �O�όv�Z
	* @param Vector3 pos0 ���_���W0
	* @param Vector3 pos1 ���_���W1
	* @return float �O�όv�Z����
	*************************************************/
	float Dot(Vector3* pPos0, Vector3* pPos1);
	
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