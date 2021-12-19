#include "../alternative.h"

bool KeyHelper::IsKeyPressed(int iVkCode)
{
	return GetAsyncKeyState(iVkCode);
}

bool KeyHelper::IsKeyDowned(int iVkCode)
{
	static bool bKeyMapDowned[255];
	bool bIsPressed = GetAsyncKeyState(iVkCode);
	bool ret = bIsPressed && !bKeyMapDowned[iVkCode];
	bKeyMapDowned[iVkCode] = bIsPressed;
	return ret;
}

bool KeyHelper::IsKeyReleased(int iVkCode)
{
	static bool bKeyMapReleased[255];
	bool bIsPressed = GetAsyncKeyState(iVkCode);
	bool ret = !bIsPressed && bKeyMapReleased[iVkCode];
	bKeyMapReleased[iVkCode] = bIsPressed;
	return ret;
}
