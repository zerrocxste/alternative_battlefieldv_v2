#include "../alternative.h"

using fsub1420C7C90 = void(__fastcall*)(__int64);
fsub1420C7C90 pfsub1420C7C90 = nullptr;

void __fastcall sub1420C7C90_hooked(__int64 a1) //a1 cse structure
{
	if (Vars::g_pVars->m_EngineVars.m_bRadarActive)
		Features::g_pFeatures->ClientSoldierEntityListGrabber((DWORD_PTR)(a1 - 0x338));

	pfsub1420C7C90(a1);
}

using fsub140970280 = __int64(__fastcall*)(__int64, __int64, __int64);
fsub140970280 pfsub140970280 = nullptr;

__int64 __fastcall sub140970280_hooked(__int64 a1, __int64 a2, __int64 a3)
{
	if (Vars::g_pVars->m_EngineVars.m_bRadarActive)
		Features::g_pFeatures->LocalpClientSoldierEntityGrabber((DWORD_PTR)(a1 + 0x110));

	return pfsub140970280(a1, a2, a3);
}

namespace HookManager
{
	CHookManager::CHookManager()
	{
		memset(this, 0, sizeof(*this));
	}

	CHookManager::~CHookManager()
	{

	}

	bool CHookManager::Hook_sub1420C7C90(bool bDoHook)
	{
		void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
			"\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x00\x48\x8B\x00\x48\x8B\x00\x48\x8B\x00\xFF\x90", "xx???xxxx?xx??xx?xx?xx?xx");

		if (bDoHook)
		{
			if (MH_CreateHook(Address, sub1420C7C90_hooked, (LPVOID*)&pfsub1420C7C90) != MH_OK)
				return false;

			if (MH_EnableHook(Address) != MH_OK)
				return false;
		}
		else
		{
			if (MH_DisableHook(Address) != MH_OK)
				return false;

			if (MH_DisableHook(Address) != MH_OK)
				return false;
		}

		return true;
	}

	bool CHookManager::Hook_sub140970280(bool bDoHook)
	{
		void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
			"\x40\x00\x57\x41\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x28\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x4C\x8B", "x?xx?xxx?xxxxx????xx???xx???xx?xx");

		if (bDoHook)
		{
			if (MH_CreateHook(Address, sub140970280_hooked, (LPVOID*)&pfsub140970280) != MH_OK)
				return false;

			if (MH_EnableHook(Address) != MH_OK)
				return false;
		}
		else
		{
			if (MH_DisableHook(Address) != MH_OK)
				return false;

			if (MH_DisableHook(Address) != MH_OK)
				return false;
		}

		return true;
	}

	bool CHookManager::DoInitialize()
	{
		if (MH_Initialize() != MH_OK)
		{
			Console::PrintLogTime(__FUNCTION__, "Minhook initialize error\n");
			return false;
		}

		if (!Hook_sub1420C7C90(true))
		{
			Console::PrintLogTime(__FUNCTION__, "sub1420C7C90 hook failed\n");
			return false;
		}

		if (!Hook_sub140970280(true))
		{
			Console::PrintLogTime(__FUNCTION__, "sub140970280 hook failed\n");
			return false;
		}

		return true;
	}

	bool CHookManager::DoUninitialize()
	{
		if (!Hook_sub1420C7C90(false))
		{
			Console::PrintLogTime(__FUNCTION__, "sub1420C7C90 unhook failed\n");
			return false;
		}

		if (!Hook_sub140970280(false))
		{
			Console::PrintLogTime(__FUNCTION__, "sub140970280 unhook failed\n");
			return false;
		}

		return MH_Uninitialize() != MH_OK;
	}

	std::unique_ptr<CHookManager> g_pHookManager = std::make_unique<CHookManager>();
}