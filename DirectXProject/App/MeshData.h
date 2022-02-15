#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Mesh.h>
#include <Vector.h>
#include <map>

/*
* @brief メッシュデータをすべて格納するクラス
*/
class MeshData : public Singleton<MeshData>
{

public:
	friend class Singleton<MeshData>;

	// メッシュの情報
	struct Info
	{
		std::shared_ptr<DrawBuffer> pDrawBuffer; // 描画バッファ
		Mesh::SurfaceList surfaceList; // ポリゴン情報(頂点3つを法線1つ)
		PrimitiveType::Kind type = PrimitiveType::TRIANGLELIST; // プリミティブタイプ
	};

public:
	void Initialize()override;
	void Finalize()override;

	/*
	* @brief ファイルを検索しメッシュを作成
	* @param str ファイル名
	*/
	void Load(std::string str);

	/*
	* @brief メモリに確保してあるメッシュのアドレスを渡す
	* @param str モデル名
	*/
	const Info* Get(std::string str);

protected:
	MeshData() {}
	virtual ~MeshData() {}

private:
	// 名前で判別できるようにmap配列を使用
	std::map<std::string, Info> m_ModelMap;
};
