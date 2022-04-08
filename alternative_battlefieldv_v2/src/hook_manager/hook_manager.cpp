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
	if (Vars::pVars->m_HackVars.m_bIncreaseFireRate &&
		(param_5 == 48 /*soldier*/ || param_5 == 80 /*not soldier*/))
	{
		static int iSkippedSound = 0;
		static __int64 LastArg1 = 0;

		iSkippedSound++;

		if (!Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased && 
			param_1 == LastArg1 &&
			 iSkippedSound < 5)
			return;
		else
			iSkippedSound = 0;

		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = false;

		LastArg1 = param_1;
	}

	pfsub14958F0D0(param_1, param_2, param_3, param_4, param_5);
}

using FUN_1405c10a0 = __int64(__fastcall*)(__int64, __int64, __int64, __int64);
FUN_1405c10a0 pFUN_1405c10a0 = nullptr;

__int64 __fastcall FUN_1405c10a0_hooked(__int64 param_1, __int64 param_2, __int64 param_3, __int64 param_4)
{
	auto ret = pFUN_1405c10a0(param_1, param_2, param_3, param_4);

	if (ret != 0)
		Features::pFeatures->DrawScreen(ret);

	return ret;
}

namespace HookManager
{
	CHookManager::CHookManager() : IError()
	{
		
	}

	CHookManager::~CHookManager()
	{
		UnhookAll();
	}

	bool CHookManager::EnableHook(const char* szPattern, const char* szMask, void* pProxyFunction, void* pGateFunction, std::uintptr_t* pFuncAddress, HMODULE hModule)
	{
		void* Address = (void*)memory_utils::pattern_scanner_module(
			hModule,
			szPattern,
			szMask);

		if (!Address) 
		{
			this->SetError("Pattern: %s, Mask: %s -> Not found", szPattern, szMask);
			return false;
		}

		if (pFuncAddress != nullptr)
			*pFuncAddress = (std::uintptr_t)Address;

		auto SetupStatus = MH_CreateHook(Address, pProxyFunction, (LPVOID*)pGateFunction);

		if (SetupStatus != MH_OK) 
		{
			this->SetError("Failed hook func, minhook status: %s, hook address: 0x%p", MH_StatusToString(SetupStatus), Address);
			return false;
		}

		auto EnableStatus = MH_EnableHook(Address);

		if (EnableStatus != MH_OK) 
		{
			this->SetError("Failed enable hook, minhook status: %s, hook address: 0x%p", MH_StatusToString(EnableStatus), Address);
			return false;
		}

		return true;
	}

	bool CHookManager::DisableHook(void* Address)
	{
		if (MH_DisableHook(Address) != MH_OK)
			return false;

		if (MH_RemoveHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::Hook_sub1420C7C90()
	{
		return EnableHook(
			"\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x00\x48\x8B\x00\x48\x8B\x00\x48\x8B\x00\xFF\x90",
			"xx???xxxx?xx??xx?xx?xx?xx",
			sub1420C7C90_hooked,
			&pfsub1420C7C90);
	}

	bool CHookManager::Hook_sub140970280()
	{
		return EnableHook(
			"\x40\x00\x57\x41\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x28\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x4C\x8B",
			"x?xx?xxx?xxxxx????xx???xx???xx?xx",
			sub140970280_hooked,
			&pfsub140970280);
	}

	bool CHookManager::Hook_sub14958F0D0()
	{
		return EnableHook(
			"\x44\x0F\x00\x00\x00\x00\x00\x00\x41\xF6\xC1\x00\x75",
			"xx??????xxx?x",
			sub14958F0D0_hooked,
			&pfsub14958F0D0);
	}

	bool CHookManager::Hook_sub1405C10A0()
	{
		return EnableHook(
			"\x40\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x85\x00\x75",
			"x?xxx?xxxxx????xx???xx???xx???xx?????xx?x",
			FUN_1405c10a0_hooked,
			&pFUN_1405c10a0);
	}

	bool CHookManager::DoInitialize()
	{
		if (MH_Initialize() != MH_OK)
		{
			this->SetError("Minhook init error");
			return false;
		}

		if (!Hook_sub1420C7C90() || 
			!Hook_sub140970280() || 
			!Hook_sub14958F0D0() || 
			!Hook_sub1405C10A0())
		{
			return false;
		}

		return true;
	}

	bool CHookManager::UnhookAll()
	{
		auto ret = DisableHook(MH_ALL_HOOKS);

		MH_Uninitialize();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		return ret;
	}

	void CHookManager::ShowError()
	{
		MessageBox(0, this->What(), "HookManager", MB_OK);
	}

	std::unique_ptr<CHookManager> pHookManager = std::make_unique<CHookManager>();
}