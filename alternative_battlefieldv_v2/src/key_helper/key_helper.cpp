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
		bool bIsPressed = GetAsyncKeyState(iVkCode);
		bool ret = !bIsPressed && this->m_bKeyMap[iVkCode];
		this->m_bKeyMap[iVkCode] = bIsPressed;
		return ret;
	}

	bool CKeyHelper::IsKeyDowned(int iVkCode)
	{
		bool bIsPressed = GetAsyncKeyState(iVkCode);
		bool ret = bIsPressed && !this->m_bKeyMap[iVkCode];
		this->m_bKeyMap[iVkCode] = bIsPressed;
		return ret;
	}

	std::unique_ptr<CKeyHelper> pKeyHelper = std::make_unique<CKeyHelper>();
}