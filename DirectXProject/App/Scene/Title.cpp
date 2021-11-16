#include <App/Scene/Title.h>
#include <System/Geometory.h>
#include <App/Main.h>

Title::Title()
{
}

Title::~Title()
{
}

void Title::Start()
{

	m_pCamera = new Camera();
	m_pLight = new Light();
}

void Title::Init()
{
	m_pLight->SetDistance(40);
	m_pCamera->Init(true);
	for (auto pObj : m_pObjList)
	{
		pObj->Init();
	}
}

void Title::Uninit()
{
	delete m_pLight;
	m_pLight = nullptr;
	m_pCamera->Uninit();
	delete m_pCamera;
	m_pCamera = nullptr;
	for (auto pObj = m_pObjList.begin(); pObj != m_pObjList.end();)
	{
		(*pObj)->Uninit();
		delete *pObj;
		*pObj = nullptr;
		pObj = m_pObjList.erase(pObj);
		continue;
		if (pObj == m_pObjList.end())
			break;
		++pObj;
	}
}

Scene_Type Title::Update(Scene_Type type)
{
	bool input = IsKeyRelease(VK_LBUTTON);

	for (auto pObj : m_pObjList)
	{
		pObj->Update();
	}

	m_pLight->Update();
	m_pCamera->RotateUpdate();
	return type;
}

void Title::Draw()
{
	//XMMATRIX matrix;
	//RENDERTARGET->Draw(RT_WORLD);

	//m_pLight->Draw();
	//SHADER->Write(CB_P_REGIST0);
	//SHADER->Write(CB_VP);

	//// •`‰æ
	//m_pLight->Bind();
	//m_pCamera->Bind3D(false);
	//SHADER->BindVS(VS_NORMAL);
	////SHADER->BindPS(PS_TRIPLANER);


	//BeginDrawDX();
	//m_pCamera->Bind2D();
	//SHADER->BindVS(VS_NORMAL);
	//SHADER->BindPS(PS_BLUR);

	//SHADER->SetTexture(RENDERTARGET->GetTexture(RT_WORLD), ShaderResource::TEX_BLUR);
	//matrix = DirectX::XMMatrixScaling(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	//matrix *= DirectX::XMMatrixTranslation(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 100);
	//SHADER->SetWorld(matrix);
	//DrawPolygon();


}
