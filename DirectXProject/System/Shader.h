#pragma once
/*DirectXの描画について...
画面への表示はGPUが行う。
プログラムの処理に関してはCPUが行う。
DIrectXはCPUからGPUへのデータの橋渡しを行う
最終的に表示するためには、3Dモデルの情報だけではなく、
どうやって画面に表示するか(影は必要か、テクスチャは貼るか、2D?3D?なのか)
の指示書が必要になる。
この指示書の事を"シェーダー"と呼ぶ
バーテックスシェーダー(VertexShader)
...モデルの頂点(ローカル座標)が画面のどの位置に表示されるかを計算
ピクセルシェーダー(PixelShader)
...画面の1ピクセルごとにどの色で表示するか計算を行う
*/
#include "DirectX.h"

// シェーダーの基本クラス
class ShaderBase
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// シェーダコンパイル(*.cso)を読み込む処理
	HRESULT Create(const char* FileName);

protected:
	// シェーダーファイルを読み込んだ後、
	// シェーダー種類別に処理を行う
	virtual HRESULT MakeShader(void* pData, UINT size, int num = 0) = 0;

	// シェーダーを描画に使用
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
// 定数バッファ
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
