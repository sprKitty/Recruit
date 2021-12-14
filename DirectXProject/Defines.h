#pragma once
#include <assert.h>
#include <Windows.h>
#include <vector>
#include <memory>

#define APP_TITLE "GAME"

// âÊñ ÉTÉCÉY
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define HALFSCREEN_WIDTH (SCREEN_WIDTH / 2)
#define HALFSCREEN_HEIGHT (SCREEN_HEIGHT / 2)

constexpr float DEVISIONX = 9.0f / HALFSCREEN_WIDTH;
constexpr float DEVISIONY = 22.5f / HALFSCREEN_HEIGHT;

#define DEBUG ((bool)false)

#define PTRNULLCHECK(ptr) ((ptr.use_count() == 0))

