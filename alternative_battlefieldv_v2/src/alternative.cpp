#include "alternative.h"

void MainRoutine()
{
	if (KeyHelper::IsKeyReleased(VK_LBUTTON))
		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = true;

	if (Vars::pVars->m_HackVars.m_bRadarActive)
		Features::pFeatures->DrawEnemyInGameRadar();
}

void MainHackThread(void* arg)
{
	std::srand(std::time(nullptr));

#ifdef _USERDEBUG
	Console::Attach("alternative | battlefield v");
#endif

	Console::PrintLogTime(false, __FUNCTION__, "Attach success");

	if (!HookManager::pHookManager->DoInitialize())
	{
		Console::PrintLogTime(true, __FUNCTION__, "Failed installation hooks!, Exit...\n");
		goto failed_jmp;
	}

	while (!GetAsyncKeyState(VK_DELETE))
	{
		MainRoutine();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	if (!HookManager::pHookManager->UnhookAll())
	{
		Console::PrintLogTime(true, __FUNCTION__, "Unhook failed!\n");
		return;
	}

	Console::PrintLogTime(false, __FUNCTION__, "Exit...\n");
	failed_jmp:
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