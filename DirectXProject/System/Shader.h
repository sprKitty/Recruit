#pragma once
#include "DirectX.h"

// �V�F�[�_�[�̊�{�N���X
class ShaderBase
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// �V�F�[�_�R���p�C��(*.cso)��ǂݍ��ޏ���
	HRESULT Create(const char* FileName);

protected:
	// �V�F�[�_�[�t�@�C����ǂݍ��񂾌�A
	// �V�F�[�_�[��ޕʂɏ������s��
	virtual HRESULT MakeShader(void* pData, UINT size, int num = 0) = 0;

	// �V�F�[�_�[��`��Ɏg�p
	virtual void Bind(void) = 0;
private:
};

class VertexShader:public ShaderBase
{
public:
	enum Layout
	{
		NORMAL,
		ANIMATION,

		MAX,
	};
public:
	VertexShader();
	~VertexShader()override;

	void Bind(void);
	void SetLayoutType(Layout type) { m_type = type; }
private:
	HRESULT MakeShader(void* pData, UINT size, int num = 0);
	Layout m_type;

	ID3D11VertexShader* m_pVS;
	ID3D11InputLayout* m_pInputLayOut;

};

class PixelShader :public ShaderBase
{
public:
	PixelShader();
	~PixelShader()override;

	void Bind(void);
private:
	HRESULT MakeShader(void* pData, UINT size, int num = 0);


	ID3D11PixelShader* m_pPS;

};

class GeometoryShader:public ShaderBase
{
public:
	GeometoryShader();
	~GeometoryShader()override;

	void Bind(void);

private:
	HRESULT MakeShader(void* pData, UINT size, int num = 0);


	ID3D11GeometryShader* m_pGS;
};

//----------
// �萔�o�b�t�@
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	HRESULT Create(UINT size);
	void Write(void* pData);
	void BindVS(UINT slot);
	void BindPS(UINT slot);
	void BindGS(UINT slot);

private:
	ID3D11Buffer* m_pBuffer;
};
