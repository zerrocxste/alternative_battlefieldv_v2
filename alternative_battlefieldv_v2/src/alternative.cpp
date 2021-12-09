#include "alternative.h"

void MainRoutine()
{
	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_LBUTTON))
		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = true;

	if (KeyHelper::pKeyHelper->IsKeyReleased(VK_F1))
		Vars::pVars->m_HackVars.m_bRadarActive = !Vars::pVars->m_HackVars.m_bRadarActive;

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