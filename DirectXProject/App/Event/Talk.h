#pragma once
#include <App/Event/EventBase.h>
#include <vector>
#include <Vector.h>
#include <map>
#include <string>

class MessageWindow;

class Talk : public EventBase
{
public:
	Talk()
		: m_nStep(0)
	{
	}
	virtual ~Talk() {}

	void Load(const char* pPath)override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetMessageWindow(std::weak_ptr<MessageWindow> pMW)
	{
		m_pMSWindow = pMW;
	}

private:
	struct Info
	{
		std::vector<Vector2> vTitle;
		std::vector<Vector2> vMain;
	};

private:
	int m_nStep;
	int m_nShowNum;
	float m_fSpeed;
	float m_fShowTime;
	std::vector<Info> m_Infos;
	std::weak_ptr<MessageWindow> m_pMSWindow;
};