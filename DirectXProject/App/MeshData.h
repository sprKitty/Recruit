#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Mesh.h>
#include <Vector.h>
#include <map>

/*
* @brief ���b�V���f�[�^�����ׂĊi�[����N���X
*/
class MeshData : public Singleton<MeshData>
{

public:
	friend class Singleton<MeshData>;

	// ���b�V���̏��
	struct Info
	{
		std::shared_ptr<DrawBuffer> pDrawBuffer; // �`��o�b�t�@
		Mesh::SurfaceList surfaceList; // �|���S�����(���_3��@��1��)
		PrimitiveType::Kind type = PrimitiveType::TRIANGLELIST; // �v���~�e�B�u�^�C�v
	};

public:
	void Initialize()override;
	void Finalize()override;

	/*
	* @brief �t�@�C�������������b�V�����쐬
	* @param str �t�@�C����
	*/
	void Load(std::string str);

	/*
	* @brief �������Ɋm�ۂ��Ă��郁�b�V���̃A�h���X��n��
	* @param str ���f����
	*/
	const Info* Get(std::string str);

protected:
	MeshData() {}
	virtual ~MeshData() {}

private:
	// ���O�Ŕ��ʂł���悤��map�z����g�p
	std::map<std::string, Info> m_ModelMap;
};
