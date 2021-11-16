#include <System/Input.h>
#include <Defines.h>


//--- �萔
const int KEYTABLE_SIZE = 256;
const int KEY_REPEAT_TICK = 3;
const int KEY_REPEAT_WAIT = 25;

// �L�[�{�[�h
//--- �O���[�o��
BYTE g_key[KEYTABLE_SIZE];
BYTE g_oldKey[KEYTABLE_SIZE];
BYTE g_repeatCount[KEYTABLE_SIZE];
BYTE g_force[KEYTABLE_SIZE];


// �}�E�X
//--- �v���g�^�C�v�錾
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

//--- �O���[�o��
HWND g_mouseHWnd;
HHOOK g_mouseHook;
int g_wheelCount;
int g_wheelValue;
bool g_doubleClickMsg[3];
bool g_doubleClick[3];
POINT g_mouseClientPos;
POINT g_mousePos;
POINT g_mouseOldPos;
/**
 * @brief �}�E�X���b�Z�[�W�t�b�N
 */
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
	}

	MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	switch (pMsg->message)
	{
		// �z�C�[��
	case WM_MOUSEWHEEL:
		g_wheelCount += GET_WHEEL_DELTA_WPARAM(pMsg->wParam) / WHEEL_DELTA;
		break;
		// ���_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		g_doubleClickMsg[0] = true;
		break;
		// ���_�u���N���b�N
	case WM_MBUTTONDBLCLK:
		g_doubleClickMsg[1] = true;
		break;
		// �E�_�u���N���b�N
	case WM_RBUTTONDBLCLK:
		g_doubleClickMsg[2] = true;
		break;
	}
	return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

HRESULT InitInput()
{
	// �L�[�{�[�h
	ZeroMemory(g_key, KEYTABLE_SIZE);
	ZeroMemory(g_oldKey, KEYTABLE_SIZE);
	ZeroMemory(g_repeatCount, KEYTABLE_SIZE);
	memset(g_force, 255, KEYTABLE_SIZE);

	// �}�E�X
	g_mouseHWnd = GetActiveWindow();
	g_mouseHook = SetWindowsHookEx(WH_GETMESSAGE, MouseProc, 0, GetCurrentThreadId());
	GetCursorPos(&g_mousePos);
	GetCursorPos(&g_mouseClientPos);
	g_mouseOldPos = g_mousePos;

	return S_OK;
}
void UninitInput()
{
	UnhookWindowsHookEx(g_mouseHook);
}
void UpdateInput()
{
	// �L�[�{�[�h
	memcpy(g_oldKey, g_key, sizeof(g_key));
	GetKeyboardState(g_key);
	for (int i = 0; i < KEYTABLE_SIZE; ++i)
	{
		if (g_force[i] != 255)
		{
			g_key[i] = g_force[i];
			g_force[i] = 255;
		}

		if (IsKeyPress(i))
		{
			if (g_repeatCount[i] <= KEY_REPEAT_WAIT)
			{
				++g_repeatCount[i];
			}
			else
			{
				g_repeatCount[i] -= KEY_REPEAT_TICK;
			}
		}
		else
		{
			g_repeatCount[i] = 0;
		}
	}

	// �}�E�X���W
	g_mouseOldPos = g_mousePos;
	GetCursorPos(&g_mousePos);
	GetCursorPos(&g_mouseClientPos);
	ScreenToClient(g_mouseHWnd, &g_mouseClientPos);
	// �}�E�X�z�C�[��
	g_wheelValue = g_wheelCount;
	g_wheelCount = 0;
	// �}�E�X�N���b�N
	for (int i = 0; i < 3; ++i)
	{
		g_doubleClick[i] = g_doubleClickMsg[i];
		g_doubleClickMsg[i] = false;
	}
}


void InstantForceInput(int nKey, bool bPress)
{
	g_force[nKey] = bPress ? 0x80 : 0;
}

bool IsKeyPress(int nKey)
{
	return g_key[nKey] & 0x80;
}
bool IsKeyTrigger(int nKey)
{
	return (g_key[nKey] ^ g_oldKey[nKey]) & g_key[nKey] & 0x80;
}
bool IsKeyRelease(int nKey)
{
	return (g_key[nKey] ^ g_oldKey[nKey]) & g_oldKey[nKey] & 0x80;
}
bool IsKeyRepeat(int nKey)
{
	return g_repeatCount[nKey] == 1 || g_repeatCount[nKey] >= KEY_REPEAT_WAIT;
}

void ShowMouseCursol()
{
	ShowCursor(true);
}
void ResetMousePos()
{
	//ShowCursor(false);
	g_mousePos.x = SCREEN_WIDTH / 2;
	g_mousePos.y = SCREEN_HEIGHT / 2;
	SetCursorPos(g_mousePos.x, g_mousePos.y);
}
int GetMouseMoveX()
{
	return g_mousePos.x - g_mouseOldPos.x;
}
int GetMouseMoveY()
{
	return g_mousePos.y - g_mouseOldPos.y;
}

int GetMousePosX()
{
	return g_mouseClientPos.x;
}

int GetMousePosY()
{
	return g_mouseClientPos.y;
}

int GetMouseWheel()
{
	return g_wheelValue;
}

bool IsMousePress(int key)
{
	switch (key)
	{
	case VK_LBUTTON:
	case VK_MBUTTON:
	case VK_RBUTTON:
		return GetAsyncKeyState(key);
		break;
	default:
		return false;
	}
}

bool IsMouseDoubleClick(int nKey)
{
	switch (nKey)
	{
	default: return false;
	case VK_LBUTTON: return g_doubleClick[0];
	case VK_MBUTTON: return g_doubleClick[1];
	case VK_RBUTTON: return g_doubleClick[2];
	}
}