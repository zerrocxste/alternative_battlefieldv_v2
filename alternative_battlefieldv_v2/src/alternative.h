#pragma once

#define _CRT_SECURE_NO_WARNINGS

//#define _USERDEBUG

#include <Windows.h>
#include <vector>
#include <map>
#include <mutex>
#include <chrono>
#include <direct.h>
#include <intrin.h>

#include "../libs/minhook/minhook.h"
#pragma comment (lib, "libs/minhook/minhook.lib")

#include "key_helper/key_helper.h"
#include "console/console.h"
#include "memory_utils/memory_utils.h"

#include "vars/vars.h"
#include "Color/Color.h"

#include "hook_manager/hook_manager.h"
#include "features/features.h"