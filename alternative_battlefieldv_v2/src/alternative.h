#pragma once

#define _CRT_SECURE_NO_WARNINGS

//#define _USERDEBUG

#include <Windows.h>
#include <vector>
#include <map>
#include <mutex>
#include <chrono>
#include <direct.h>

#include "../libs/minhook/minhook.h"
#pragma comment (lib, "libs/minhook/minhook.lib")

#include "key_helper/key_helper.h"
#include "console/console.h"
#include "memory_utils/memory_utils.h"

#include "vars/vars.h"

#include "hook_manager/hook_manager.h"
#include "features/features.h"

        /*Address of signature = bfv.exe + 0x06675220
            "\x44\x38\x00\x00\x0F\x84\x00\x00\x00\x00\x85\xFF\x74", "xx??xx????xxx"
            "44 38 ? ? 0F 84 ? ? ? ? 85 FF 74"

            Address of signature = bfv.exe + 0x066761B0
            "\x80\x7F\x11\x00\x74\x00\x0F\x10", "xxx?x?xx"
            "80 7F 11 ? 74 ? 0F 10"*/