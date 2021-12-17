#include "../alternative.h"

using fsub1420C7C90 = void(__fastcall*)(__int64);
fsub1420C7C90 pfsub1420C7C90 = nullptr;

void __fastcall sub1420C7C90_hooked(__int64 a1) //a1 cse structure
{
	if (Vars::pVars->m_HackVars.m_bRadarActive)
		Features::pFeatures->ClientSoldierEntityListGrabber((DWORD_PTR)(a1 - 0x338));

	pfsub1420C7C90(a1);
}

using fsub140970280 = __int64(__fastcall*)(__int64, __int64, __int64);
fsub140970280 pfsub140970280 = nullptr;

__int64 __fastcall sub140970280_hooked(__int64 a1, __int64 a2, __int64 a3)
{
	if (Vars::pVars->m_HackVars.m_bRadarActive)
		Features::pFeatures->LocalpClientSoldierEntityGrabber((DWORD_PTR)(a1 + 0x110));

	return pfsub140970280(a1, a2, a3);
}

using sub14958F0D0 = void(__fastcall*)(__int64, __int64, int, __int64, byte);
sub14958F0D0 pfsub14958F0D0 = nullptr;

void __fastcall sub14958F0D0_hooked(__int64 param_1, __int64 param_2, int param_3, __int64 param_4, byte param_5)
{
	auto bFireSoundIndexIsLocal = param_5 == 48 /*soldier*/ || param_5 == 80 /*not soldier*/; //sound spam fix

	if (Vars::pVars->m_HackVars.m_bIncreaseFireRate && bFireSoundIndexIsLocal)
	{
		static int iSkippedSound = 0;
		static __int64 pLastArg1 = 0;

		iSkippedSound++;

		if (!Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased && 
			param_1 == pLastArg1 &&
			 iSkippedSound < 5)
			return;
		else
			iSkippedSound = 0;

		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = false;

		pLastArg1 = param_1;
	}

	pfsub14958F0D0(param_1, param_2, param_3, param_4, param_5);
}

using FUN_1405c10a0 = __int64(__fastcall*)(__int64, __int64, __int64, __int64);
FUN_1405c10a0 pFUN_1405c10a0 = nullptr;

__int64 FUN_1405c10a0_hooked(__int64 param_1, __int64 param_2, __int64 param_3, __int64 param_4)
{
	auto ret = pFUN_1405c10a0(param_1, param_2, param_3, param_4);

	if (ret != 0)
		Features::pFeatures->DrawScreen(ret);

	return ret;
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

	bool CHookManager::DisableHook(void* Address)
	{
		if (MH_DisableHook(Address) != MH_OK)
			return false;

		if (MH_RemoveHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::Hook_sub1420C7C90(bool bDoHook)
	{
		static void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
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
			if (!DisableHook(Address))
				return false;
		}

		return true;
	}

	bool CHookManager::Hook_sub140970280(bool bDoHook)
	{
		static void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
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
			if (!DisableHook(Address))
				return false;
		}

		return true;
	}

	bool CHookManager::Hook_sub14958F0D0(bool bDoHook)
	{
		static void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
			"\x44\x0F\x00\x00\x00\x00\x00\x00\x41\xF6\xC1\x00\x75", "xx??????xxx?x");

		if (bDoHook)
		{
			if (MH_CreateHook(Address, sub14958F0D0_hooked, (LPVOID*)&pfsub14958F0D0) != MH_OK)
				return false;

			if (MH_EnableHook(Address) != MH_OK)
				return false;
		}
		else
		{
			if (!DisableHook(Address))
				return false;
		}
		
		return true;
	}

	bool CHookManager::Hook_sub1405C10A0(bool bDoHook)
	{
		static void* Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(),
			"\x40\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x85\x00\x75", 
			"x?xxx?xxxxx????xx???xx???xx???xx?????xx?x");

		if (bDoHook)
		{
			if (MH_CreateHook(Address, FUN_1405c10a0_hooked, (LPVOID*)&pFUN_1405c10a0) != MH_OK)
				return false;

			if (MH_EnableHook(Address) != MH_OK)
				return false;
		}
		else
		{
			if (!DisableHook(Address))
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

		if (!Hook_sub14958F0D0(true))
		{
			Console::PrintLogTime(__FUNCTION__, "sub14958F0D0 hook failed\n");
			return false;
		}

		if (!Hook_sub1405C10A0(true))
		{
			Console::PrintLogTime(__FUNCTION__, "sub1405C10A0 hook failed\n");
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

		if (!Hook_sub14958F0D0(false))
		{
			Console::PrintLogTime(__FUNCTION__, "sub14958F0D0 unhook failed\n");
			return false;
		}

		if (!Hook_sub1405C10A0(false))
		{
			Console::PrintLogTime(__FUNCTION__, "sub1405C10A0 unhook failed\n");
			return false;
		}

		return MH_Uninitialize() == MH_OK;
	}

	std::unique_ptr<CHookManager> pHookManager = std::make_unique<CHookManager>();
}