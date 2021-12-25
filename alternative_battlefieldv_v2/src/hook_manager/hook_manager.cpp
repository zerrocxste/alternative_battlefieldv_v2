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

__int64 FUN_1405c10a0_hooked(__int64 param_1, __int64 param_2, __int64 param_3, __int64 param_4)
{
	auto ret = pFUN_1405c10a0(param_1, param_2, param_3, param_4);

	if (ret != 0)
		Features::pFeatures->DrawScreen(ret);

	return ret;
}

using FUN_14927c100 = __int64(__fastcall*)(__int64, char);
FUN_14927c100 pFUN_14927c100 = nullptr;

__int64 FUN_14927c100_hooked(__int64 param_1, char param_2)
{
	if (Vars::pVars->m_HackVars.m_bReloadInScope)
	{
		__int64 lVar1;
		bool bVar2;
		bool bVar3;
		unsigned __int64 uVar4;
		__int64 lVar5;
		unsigned __int64 uVar6;
		__int64 lVar7;
		int iVar8;

		if ((*(byte*)(param_1 + 0x28) & 4) == 0) {
			lVar5 = *(__int64*)(param_1 + 0xc10);
		}
		else {
			lVar5 = *(__int64*)(param_1 + 0xc18);
		}
		if (lVar5 == 0) {
			uVar6 = 0;
		}
		else {
			uVar6 = (__int64)*(unsigned int*)(lVar5 + 8);
		}

		uVar4 = *(__int64*)(param_1 + 3000) - *(__int64*)(param_1 + 0xbb0) >> 3;
		if ((unsigned int)uVar4 <= (unsigned int)uVar6) {
		LAB_14927c278:
			return uVar4 & 0xffffffffffffff00;
		}
		uVar4 = *(unsigned __int64*)(param_1 + 0xbb0);
		lVar5 = *(__int64*)(uVar4 + uVar6 * 8);
		if ((lVar5 == 0) || ((*(byte*)(lVar5 + 0x1b) & 1) == 0)) goto LAB_14927c278;
		lVar1 = *(__int64*)(param_1 + 0x20);
		if ((lVar1 == 0) || (lVar1 == 0x38)) {
			lVar7 = 0;
		}
		else {
			lVar7 = *(__int64*)(lVar1 + 0x358);
		}
		bVar2 = true;
		if ((lVar7 == 0) || (*(char*)(lVar7 + 0x1c39) == '\0')) {
			bVar3 = false;
		}
		else {
			bVar3 = true;
		}
		if (!bVar3) {
			uVar6 = *(unsigned __int64*)(lVar5 + 0x5f28);
			iVar8 = *(int*)(uVar6 + 0xdf8);
			goto LAB_14927c263;
		}
		if ((lVar1 == 0) || (lVar1 == 0x38)) {
			lVar5 = 0;
		}
		else {
			lVar5 = *(__int64*)(lVar1 + 0x358);
		}
		if ((lVar5 == 0) || (*(char*)(lVar5 + 0x1c39) == '\0')) {
			uVar6 = 0;
		}
		else {
			uVar6 = 1;
		}
		if ((char)uVar6 == '\0') {
		LAB_14927c244:
			return uVar6 & 0xffffffffffffff00;
		}
		uVar6 = *(__int64*)((*(__int64*)(param_1 + 0xbf8)) + 0xa40);
		if ((uVar6 == 0) || (lVar5 = *(__int64*)(uVar6 + 0x1c8), lVar5 == 0)) goto LAB_14927c244;

		iVar8 = *(int*)(lVar5 + 0x224);
		if (iVar8)
		{
			iVar8 = !*(unsigned char*)(lVar5 + 0x250);
		}
		if (param_2 == '\0') goto LAB_14927c263;
		if (((*(byte*)(lVar5 + 0x250) & 2) != 0) || (0.0 < *(float*)(lVar5 + 0x238))) {
			uVar6 = *(__int64*)(param_1 + 0x268) - *(__int64*)(param_1 + 0x260) >> 3;
			if ((unsigned int)uVar6 < 2) goto LAB_14927c228;
		}
		else {
		LAB_14927c228:
			bVar2 = false;
		}
		if (bVar2) {
			return uVar6 & 0xffffffffffffff00 | (unsigned __int64)(iVar8 == 0);
		}
	LAB_14927c263:
		return uVar6 & 0xffffffffffffff00 | (unsigned __int64)(iVar8 != 0);
	}
	else
	{
		return pFUN_14927c100(param_1, param_2);
	}
}

namespace HookManager
{
	CHookManager::CHookManager()
	{

	}

	CHookManager::~CHookManager()
	{
		//UnhookAll();
	}

	void* CHookManager::FindFunctionMainModule(const char* pszPattern, const char* pszMask)
	{
		auto Address = (void*)memory_utils::pattern_scanner_module(memory_utils::get_base(), pszPattern, pszMask);

		if (Address)
			this->m_vFunctionsAddressessList.push_back(Address);

		return Address;
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
		static void* Address = FindFunctionMainModule("\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x00\x48\x8B\x00\x48\x8B\x00\x48\x8B\x00\xFF\x90", 
			"xx???xxxx?xx??xx?xx?xx?xx");
	
		if (!Address)
			return false;

		if (MH_CreateHook(Address, sub1420C7C90_hooked, (LPVOID*)&pfsub1420C7C90) != MH_OK)
			return false;

		if (MH_EnableHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::Hook_sub140970280()
	{
		static void* Address = FindFunctionMainModule(
			"\x40\x00\x57\x41\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x28\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x4C\x8B", 
			"x?xx?xxx?xxxxx????xx???xx???xx?xx");

		if (MH_CreateHook(Address, sub140970280_hooked, (LPVOID*)&pfsub140970280) != MH_OK)
			return false;

		if (MH_EnableHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::Hook_sub14958F0D0()
	{
		static void* Address = FindFunctionMainModule("\x44\x0F\x00\x00\x00\x00\x00\x00\x41\xF6\xC1\x00\x75", "xx??????xxx?x");

		if (MH_CreateHook(Address, sub14958F0D0_hooked, (LPVOID*)&pfsub14958F0D0) != MH_OK)
			return false;

		if (MH_EnableHook(Address) != MH_OK)
			return false;
		
		return true;
	}

	bool CHookManager::Hook_sub1405C10A0()
	{
		static void* Address = FindFunctionMainModule(
			"\x40\x00\x48\x83\xEC\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x85\x00\x75", 
			"x?xxx?xxxxx????xx???xx???xx???xx?????xx?x");

		if (MH_CreateHook(Address, FUN_1405c10a0_hooked, (LPVOID*)&pFUN_1405c10a0) != MH_OK)
			return false;

		if (MH_EnableHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::Hook_sub14927c100()
	{
		static void* Address = FindFunctionMainModule(
			"\x40\x00\x48\x83\xEC\x00\xF6\x41\x28\x00\x0F\xB6\x00\x49\x89\x00\x74\x00\x48\x8B\x00\x00\x00\x00\x00\xEB\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x85\x00\x75\x00\x45\x31\x00\xEB\x00\x44\x8B\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x2B\x00\x00\x00\x00\x00\x48\xC1\xF8\x00\x41\x39\x00\x0F\x83", 
			"x?xxx?xxx?xx?xx?x?xx?????x?xx?????xx?x?xx?x?xx??xx?????xx?????xxx?xx?xx");

		if (MH_CreateHook(Address, FUN_14927c100_hooked, (LPVOID*)&pFUN_14927c100) != MH_OK)
			return false;

		if (MH_EnableHook(Address) != MH_OK)
			return false;

		return true;
	}

	bool CHookManager::DoInitialize()
	{
		if (MH_Initialize() != MH_OK)
		{
			Console::PrintLogTime(true, __FUNCTION__, "Minhook initialize error\n");
			return false;
		}

		if (!Hook_sub1420C7C90())
		{
			Console::PrintLogTime(true, __FUNCTION__, "sub1420C7C90 hook failed\n");
			return false;
		}

		if (!Hook_sub140970280())
		{
			Console::PrintLogTime(true, __FUNCTION__, "sub140970280 hook failed\n");
			return false;
		}

		if (!Hook_sub14958F0D0())
		{
			Console::PrintLogTime(true, __FUNCTION__, "sub14958F0D0 hook failed\n");
			return false;
		}

		if (!Hook_sub1405C10A0())
		{
			Console::PrintLogTime(true, __FUNCTION__, "sub1405C10A0 hook failed\n");
			return false;
		}

		/*if (!Hook_sub14927c100())
		{
			Console::PrintLogTime(true, __FUNCTION__, "sub14927c100 hook failed\n");
			return false;
		}*/

		return true;
	}

	bool CHookManager::UnhookAll()
	{
		auto ret = true;

		for (auto& addr : this->m_vFunctionsAddressessList)
		{
			if (!DisableHook(addr))
			{
				Console::PrintLogTime(true, __FUNCTION__, "Failed unhook at: 0x%p\n", addr);
				ret = false;
			}
		}

		MH_Uninitialize();

		std::this_thread::sleep_for(std::chrono::microseconds(500));

		return ret;
	}

	std::unique_ptr<CHookManager> pHookManager = std::make_unique<CHookManager>();
}