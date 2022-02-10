#include <windows.h>
#include "Defines.h"
#include <App/Scene/SceneMgr.h>
#include <stdio.h>
#include <System/Clocker.h>
#include <System/DebugLog.h>

// timeGetTime周りの使用
#pragma comment(lib, "winmm.lib")

//--- プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static VOID funcSetClientSize(HWND hWnd, LONG sx, LONG sy);

// エントリポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//--- 変数宣言
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG message;

	// ウィンドクラス情報の設定
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

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to RegisterClassEx", "Error", MB_OK);
		return 0;
	}

	// ウィンドウの作成
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, wcex.lpszClassName,
		APP_TITLE, WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		HWND_DESKTOP,
		NULL, hInstance, NULL
	);


	
	DebugLog::GetInstance().Initialize();
	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	funcSetClientSize(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	// 初期化処理
	if (FAILED(SceneMgr::GetInstance().Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		return 0;
	}

	Clocker::GetInstance().Initialize();

	//--- FPS制御
	int fpsCount = 0;	// 1秒間の処理回数
	timeBeginPeriod(1);
	DWORD countStartTime = timeGetTime();
	DWORD preExecTime = countStartTime;
	Clocker::GetInstance().StartFrame(countStartTime);
	//--- ウィンドウの管理
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
			// FPSの制御
			DWORD nowTime = timeGetTime();	//現在時刻

			if (nowTime - countStartTime >= 1000.0f)
			{
				char fpsText[20];
				sprintf(fpsText, "Witch", fpsCount);
				SetWindowText(hWnd, fpsText);
				// 次の１秒間の計測準備
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


	// 終了時
	timeEndPeriod(1);
	SceneMgr::GetInstance().Finalize();

	DebugLog::GetInstance().Finalize();

	UnregisterClass(wcex.lpszClassName, hInstance);


	return 0;
}

// ウィンドウプロシージャ
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
			// [x]が押されたように振舞う
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