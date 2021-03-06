#pragma once
#include <System/ClassDesign/Manager.h>
#include <System/ClassDesign/Delegate.h>
#include <App/Component/Object.h>
#include <Vector.h>
#include <string>
#include <vector>
#include <map>

class Renderer2D;
class ShaderBuffer;

class MessageWindow : public Manager
{
public:
	MessageWindow() {}
	virtual ~MessageWindow() {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Bind(const std::vector<Vector2>& vTitle, const std::vector<Vector2>& vMain, const int nFontSize = 64, const int nAnimation = -1);
	/*
	* @brief 文字列をセット
	* @param str 描画する文字
	* @param nAnimation 文字を何文字描画するか
	*        default -1 描画文字数を設定しない
	*/
	void Draw(const std::weak_ptr<ShaderBuffer> pBuf);


	static void CreateOffsetMap();
	
	inline static const float GetShowSpeed() { return m_fShowSpeed; }

	inline static const Vector2& GetOffset(const std::string& str) { return m_OffsetMap[str]; }

private:
	struct MessageInfo
	{
		std::vector<Vector2>  vTitleOffsets;
		std::vector<Vector2>  vMainOffsets;
		int nMainFontSize;
	};

private:
	static std::map<std::string, Vector2> m_OffsetMap;
	static float m_fShowSpeed;

	Object::OWNER_OBJ m_pObject;
	std::weak_ptr<Renderer2D> m_pHiragana;
	std::weak_ptr<Renderer2D> m_pWindow;
	MessageInfo m_StackMsgInfo;
};