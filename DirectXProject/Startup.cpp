#include <windows.h>
#include "Defines.h"
#include <App/Scene/SceneMgr.h>
#include <stdio.h>
#include <System/Clocker.h>
#include <System/DebugLog.h>

// timeGetTime����̎g�p
#pragma comment(lib, "winmm.lib")

//--- �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static VOID funcSetClientSize(HWND hWnd, LONG sx, LONG sy);

// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//--- �ϐ��錾
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG message;

	// �E�B���h�N���X���̐ݒ�
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_CLASSDC | CS_DBLCLKS;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// �E�B���h�E�N���X���̓o�^
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to RegisterClassEx", "Error", MB_OK);
		return 0;
	}

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, wcex.lpszClassName,
		APP_TITLE, WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		HWND_DESKTOP,
		NULL, hInstance, NULL
	);


	
	DebugLog::GetInstance().Initialize();
	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	funcSetClientSize(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	// ����������
	if (FAILED(SceneMgr::GetInstance().Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		return 0;
	}

	Clocker::GetInstance().Initialize();

	//--- FPS����
	int fpsCount = 0;	// 1�b�Ԃ̏�����
	timeBeginPeriod(1);
	DWORD countStartTime = timeGetTime();
	DWORD preExecTime = countStartTime;
	Clocker::GetInstance().StartFrame(countStartTime);
	//--- �E�B���h�E�̊Ǘ�
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			// FPS�̐���
			DWORD nowTime = timeGetTime();	//���ݎ���

			if (nowTime - countStartTime >= 1000.0f)
			{
				char fpsText[20];
				sprintf(fpsText, "Witch", fpsCount);
				SetWindowText(hWnd, fpsText);
				// ���̂P�b�Ԃ̌v������
				countStartTime = nowTime;
				fpsCount = 0;
			}

			if (nowTime - preExecTime >= 1000.0f / 144.0f)
			{
				Clocker::GetInstance().EndFrame(timeGetTime());
				Clocker::GetInstance().CalcFrameTime();
				Clocker::GetInstance().StartFrame(timeGetTime());
				SceneMgr::GetInstance().Update();
				SceneMgr::GetInstance().Draw();
				preExecTime = nowTime;
				++fpsCount;
			}

		}
	}


	// �I����
	timeEndPeriod(1);
	SceneMgr::GetInstance().Finalize();

	DebugLog::GetInstance().Finalize();

	UnregisterClass(wcex.lpszClassName, hInstance);


	return 0;
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			// [x]�������ꂽ�悤�ɐU����
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

static VOID funcSetClientSize(HWND hWnd, LONG sx, LONG sy)
{
	RECT rc1;
	RECT rc2;

	GetWindowRect(hWnd, &rc1);
	GetClientRect(hWnd, &rc2);
	sx += ((rc1.right - rc1.left) - (rc2.right - rc2.left));
	sy += ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
	SetWindowPos(hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
}