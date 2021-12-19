#include "../alternative.h"

void FrostbiteDrawing::DrawEngineText(__int64 pUnk, int x, int y, const char* pszText, Color color, float flTextSize)
{
	static auto Address = memory_utils::pattern_scanner_module(memory_utils::get_base(),
		"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x4C\x89\x00\x44\x89\x00\x89\xD5", "xx???xx???xx???xxxx?xx?xx?xx");

	if (!Address)
		return;

	(*(void(__fastcall*)(__int64, int, int, __int64, int, float))Address)(pUnk, x, y, (__int64)pszText, color.AtByteArr(), flTextSize);
}

void FrostbiteDrawing::DrawEngineFilledRect(__int64 pUnk, float x, float y, float x1, float y1, Color color)
{
	static auto Address = memory_utils::pattern_scanner_module(memory_utils::get_base(),
		"\x48\x89\x00\x48\x89\x00\x00\x48\x89\x00\x00\x48\x89\x00\x00\x57\x48\x83\xEC\x00\x0F\x29\x00\x00\x48\x89\x00\xF3\x0F", "xx?xx??xx??xx??xxxx?xx??xx?xx");

	if (!Address)
		return;

	float pointFrom[2]{ x, y };
	float pointTo[2]{ x1, y1 };

	(*(void(__fastcall*)(__int64, float*, float*, int))Address)(pUnk, pointFrom, pointTo, color.AtByteArr());
}