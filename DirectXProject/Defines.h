#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <vector>
#include <memory>

#define APP_TITLE "GAME"

// 画面サイズ
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define HALFSCREEN_WIDTH (SCREEN_WIDTH / 2)
#define HALFSCREEN_HEIGHT (SCREEN_HEIGHT / 2)

constexpr float DEVISIONX = 9.0f / HALFSCREEN_WIDTH;
constexpr float DEVISIONY = 22.5f / HALFSCREEN_HEIGHT;

#define DEBUG ((bool)false)

#define PTRNULLCHECK(ptr) ((ptr.use_count() == 0))
// リソースパス
#define ASSET(path)	"Assets/"path

// 実行エラー
#define FnAssert(fn, str) do { _ASSERT_EXPR(SUCCEEDED(fn), _CRT_WIDE(str)); } while(0)


#endif // __DEFINES_H__