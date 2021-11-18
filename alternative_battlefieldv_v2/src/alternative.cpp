#include "alternative.h"

void MainRoutine()
{
	if (KeyHelper::g_pKeyHelper->IsKeyReleased(VK_F1))
		Vars::g_pVars->m_EngineVars.m_bRadarActive = !Vars::g_pVars->m_EngineVars.m_bRadarActive;

	if (Vars::g_pVars->m_EngineVars.m_bRadarActive)
		Features::g_pFeatures->DrawEnemyInGameRadar();

	if (KeyHelper::g_pKeyHelper->IsKeyReleased(VK_F2))
	{
		Features::g_pFeatures->PatchDrawNameTagsAlwaysVisible(
			Vars::g_pVars->m_MiscVars.m_bNameTagsAlwaysVisible = !Vars::g_pVars->m_MiscVars.m_bNameTagsAlwaysVisible
		);
	}

	if (KeyHelper::g_pKeyHelper->IsKeyReleased(VK_F3))
	{
		Features::g_pFeatures->NoRecoil(
			Vars::g_pVars->m_MiscVars.m_bNoRecoil = !Vars::g_pVars->m_MiscVars.m_bNoRecoil
		);
	}

	if (KeyHelper::g_pKeyHelper->IsKeyReleased(VK_F4))
	{
		Features::g_pFeatures->NoSpread(
			Vars::g_pVars->m_MiscVars.m_bNoSpread = !Vars::g_pVars->m_MiscVars.m_bNoSpread
		);
	}

	if (KeyHelper::g_pKeyHelper->IsKeyReleased(VK_F5))
	{
		Features::g_pFeatures->IncreaseFireRate(
			Vars::g_pVars->m_MiscVars.m_bIncreaseFirerate = !Vars::g_pVars->m_MiscVars.m_bIncreaseFirerate
		);
	}
}

void MainHackThread(void* arg)
{
	//Console::Attach("alternative | battlefield v");

	Console::PrintLogTime(__FUNCTION__, "Attach success\n");

	if (!HookManager::g_pHookManager->DoInitialize())
		goto failed_jmp;

	while (!GetAsyncKeyState(VK_DELETE))
	{
		MainRoutine();
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	if (!HookManager::g_pHookManager->DoUninitialize())
	{
		Console::PrintLogTime(__FUNCTION__, "Unhook failed!\n");
		return;
	}

	failed_jmp:
	Console::PrintLogTime(__FUNCTION__, "Exit...\n");
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