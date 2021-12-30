#include "../alternative.h"

namespace FrostbiteFunctions
{
	using fSetBlockInput = bool(__fastcall*)(bool, int);
	fSetBlockInput pfSetBlockInput = (fSetBlockInput)memory_utils::pattern_scanner_module(
		memory_utils::get_base(),
		"\x48\x8B\x00\x00\x00\x00\x00\x4C\x8B\x00\x00\x00\x00\x00\x4C\x39\x00\x74\x00\x4C\x8B\x00\x41\x39\x00\x74\x00\x48\x83\xC0\x00\x4C\x39\x00\x75\x00\xC3\x41\x88\x00\x00\xC3\x48\x8D",
		"xx?????xx?????xx?x?xx?xx?x?xxx?xx?x?xxx??xxx");

	using fDrawScreenText = void(__fastcall*)(__int64, int, int, __int64, int, float);
	fDrawScreenText pfDrawScreenText = (fDrawScreenText)memory_utils::pattern_scanner_module(
		memory_utils::get_base(),
		"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x4C\x89\x00\x44\x89\x00\x89\xD5",
		"xx???xx???xx???xxxx?xx?xx?xx");

	using fDrawEngineLine = void(__fastcall*)(__int64, float*, float*, int);
	fDrawEngineLine pfDrawEngineLine = (fDrawEngineLine)memory_utils::pattern_scanner_module(
		memory_utils::get_base(),
		"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x89\x00\x4C\x89\x00\xBA\x00\x00\x00\x00\x44\x89",
		"xx???xx???xxxx?xx?xx?x????xx");

	using fDrawEngineFilledRect = void(__fastcall*)(__int64, float*, float*, int);
	fDrawEngineFilledRect pfDrawEngineFilledRect = (fDrawEngineFilledRect)memory_utils::pattern_scanner_module(
		memory_utils::get_base(),
		"\x48\x89\x00\x48\x89\x00\x00\x48\x89\x00\x00\x48\x89\x00\x00\x57\x48\x83\xEC\x00\x0F\x29\x00\x00\x48\x89\x00\xF3\x0F",
		"xx?xx??xx??xx??xxxx?xx??xx?xx");

	using fGetGameScreenResolution = void(__fastcall*)(__int64, int*);
	fGetGameScreenResolution pfGetGameScreenResolution = (fGetGameScreenResolution)memory_utils::pattern_scanner_module(
		memory_utils::get_base(),
		"\x8B\x05\x00\x00\x00\x00\x89\x02\x8B\x05",
		"xx????xxxx");

	bool Input::SetBlockInput(bool bIsBlock)
	{
		if (!pfSetBlockInput)
			return false;

		pfSetBlockInput(bIsBlock, 0);

		return true;
	}

	void Drawing::DrawEngineText(__int64 pUnk, int x, int y, const char* pszText, Color color, float flTextSize)
	{
		if (!pfDrawScreenText || !pszText)
			return;

		pfDrawScreenText(pUnk, x, y, (__int64)pszText, color.AtByteArr(), flTextSize);
	}

	void Drawing::DrawEngineLine(__int64 pUnk, float x, float y, float x1, float y1, Color color)
	{
		if (!pfDrawEngineLine)
			return;

		float pointFrom[2]{ x, y };
		float pointTo[2]{ x1, y1 };

		pfDrawEngineLine(pUnk, pointFrom, pointTo, color.AtByteArr());
	}

	void Drawing::DrawEngineFilledRect(__int64 pUnk, float x, float y, float x1, float y1, Color color)
	{
		if (!pfDrawEngineFilledRect)
			return;

		float pointFrom[2]{ x, y };
		float pointTo[2]{ x1, y1 };

		pfDrawEngineFilledRect(pUnk, pointFrom, pointTo, color.AtByteArr());
	}

	void Drawing::DrawEngineRect(__int64 pUnk, float x, float y, float x1, float y1, Color color)
	{
		DrawEngineLine(pUnk, x, y, x1, y, color);
		DrawEngineLine(pUnk, x, y, x, y1, color);
		DrawEngineLine(pUnk, x, y1, x1, y1, color);
		DrawEngineLine(pUnk, x1, y, x1, y1, color);
	}

	int* Drawing::GetGameScreenResolution()
	{
		int iResolution[2] = { 0, 0 };

		if (!pfGetGameScreenResolution)
			return iResolution;

		pfGetGameScreenResolution(0, iResolution);

		return iResolution;
	}
}