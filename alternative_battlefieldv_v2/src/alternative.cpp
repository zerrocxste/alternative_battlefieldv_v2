#include "alternative.h"

void MainRoutine()
{
	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_LBUTTON))
		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = true;

	if (Vars::pVars->m_HackVars.m_bRadarActive)
		Features::pFeatures->DrawEnemyInGameRadar();

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F2))
	{
		Features::pFeatures->PatchDrawNameTagsAlwaysVisible(
			Vars::pVars->m_HackVars.m_bNameTagsAlwaysVisible = !Vars::pVars->m_HackVars.m_bNameTagsAlwaysVisible
		);
	}

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F3))
	{
		Features::pFeatures->PatchNameTagDrawExtendedInfo(
			Vars::pVars->m_HackVars.m_bNameTagDrawExtendedInfo = !Vars::pVars->m_HackVars.m_bNameTagDrawExtendedInfo
		);
	}

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F4))
	{
		Features::pFeatures->NoRecoil(
			Vars::pVars->m_HackVars.m_bNoRecoil = !Vars::pVars->m_HackVars.m_bNoRecoil
		);
	}

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F5))
	{
		Features::pFeatures->IncreaseFireRate(
			Vars::pVars->m_HackVars.m_bIncreaseFireRate = !Vars::pVars->m_HackVars.m_bIncreaseFireRate
		);
	}

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F6))
	{
		Features::pFeatures->PatchInScopeReloading(
			Vars::pVars->m_HackVars.m_bReloadInScope = !Vars::pVars->m_HackVars.m_bReloadInScope
		);
	}
}

void MainHackThread(void* arg)
{
#ifdef _USERDEBUG
	Console::Attach("alternative | battlefield v");
#endif

	Console::PrintLogTime(__FUNCTION__, "Attach success");

	if (!HookManager::pHookManager->DoInitialize())
	{
		Console::PrintLogTime(__FUNCTION__, "Failed installation hooks!\n");
		goto failed_jmp;
	}

	while (!GetAsyncKeyState(VK_DELETE))
	{
		MainRoutine();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	if (!HookManager::pHookManager->DoUninitialize())
	{
		Console::PrintLogTime(__FUNCTION__, "Unhook failed!\n");
		return;
	}

	failed_jmp:
	Console::PrintLogTime(__FUNCTION__, "Exit...\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	FreeLibraryAndExitThread((HMODULE)arg, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainHackThread, hModule, 0, nullptr);

	return TRUE;
}

/*
Address of signature = bfv.exe + 0x01F26007
"\x41\x80\xBF\x51\x02\x00\x00\x00\x74", "xxxxxxx?x"
"41 80 BF 51 02 00 00 ? 74"
*/

/*
Address of signature = bfv.exe + 0x020D365D
"\x41\x8B\x00\x00\x00\x00\x00\x45\x8B\x00\x00\x00\x00\x00\x41\x3B", "xx?????xx?????xx"
"41 8B ? ? ? ? ? 45 8B ? ? ? ? ? 41 3B"
*/