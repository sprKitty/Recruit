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

#define DEBUG ((bool)false)

#define PTRNULLCHECK(ptr) ((ptr.use_count() == 0))

template <class T>
using weak_ptr_list = std::vector<std::weak_ptr<T>>;

template <class T>
using shared_ptr_list = std::vector<std::shared_ptr<T>>;
