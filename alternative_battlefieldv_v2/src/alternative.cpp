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

	Vars::pVars->m_CoreVars.hThisModule = (HMODULE)arg;

#ifdef _USERDEBUG
	Console::Attach("alternative | battlefield v");
#endif

	Console::PrintLogTime(__FUNCTION__, "Attach success");

	if (!HookManager::pHookManager->DoInitialize())
	{
		Console::Attach("Error");
		Console::PrintLogTime(__FUNCTION__, "Failed installation hooks!\n");
		goto failed_jmp;
	}

	while (!KeyHelper::IsKeyPressed(VK_DELETE))
	{
		MainRoutine();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}

	FrostbiteFunctions::Input::SetBlockInput(false);

failed_jmp:
	Console::PrintLogTime(__FUNCTION__, "Exit...\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	FreeLibraryAndExitThread(Vars::pVars->m_CoreVars.hThisModule, EXIT_SUCCESS);
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