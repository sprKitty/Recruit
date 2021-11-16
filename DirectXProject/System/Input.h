#pragma once
#include <Windows.h>

HRESULT InitInput();
void UninitInput();
void UpdateInput();

void InstantForceInput(int nKey, bool bPress);

bool IsKeyPress(int nKey);
bool IsKeyTrigger(int nKey);
bool IsKeyRelease(int nKey);
bool IsKeyRepeat(int nKey);

void ShowMouseCursol();
void ResetMousePos();
int GetMouseMoveX();
int GetMouseMoveY();
bool IsMousePress(int key);
bool IsMouseDoubleClick(int nKey);
int GetMousePosX();
int GetMousePosY();
int GetMouseWheel();