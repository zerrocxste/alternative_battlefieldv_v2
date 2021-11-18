#include "../alternative.h"

namespace KeyHelper
{
	CKeyHelper::CKeyHelper()
	{
		memset(this, 0, sizeof(*this));
	}

	CKeyHelper::~CKeyHelper()
	{

	}

	bool CKeyHelper::IsKeyReleased(int iVkCode)
	{
		static bool bKeyMap[255] = { 0 };
		bool bIsPressed = GetAsyncKeyState(iVkCode);
		bool ret = !bIsPressed && bKeyMap[iVkCode];
		bKeyMap[iVkCode] = bIsPressed;
		return ret;
	}

	bool CKeyHelper::IsKeyDowned(int iVkCode)
	{
		static bool bKeyMap[255] = { 0 };
		bool bIsPressed = GetAsyncKeyState(iVkCode);
		bool ret = bIsPressed && !bKeyMap[iVkCode];
		bKeyMap[iVkCode] = bIsPressed;
		return ret;
	}

	std::unique_ptr<CKeyHelper> g_pKeyHelper = std::make_unique<CKeyHelper>();
}