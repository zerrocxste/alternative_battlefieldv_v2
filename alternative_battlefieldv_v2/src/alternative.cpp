#include "alternative.h"

void MainRoutine()
{
	if (KeyHelper::IsKeyReleased(VK_LBUTTON))
		Vars::pVars->m_BackendVars.m_bUpdatedSoundLButtonReleased = true;

	if (Vars::pVars->m_HackVars.m_bRadarActive)
		Features::pFeatures->DrawEnemyInGameRadar();
}

bool Initiating(HMODULE hThisModule)
{
	std::srand(std::time(nullptr));

	Vars::pVars->m_CoreVars.hThisModule = hThisModule;

#ifdef _USERDEBUG
	Console::Attach("alternative | battlefield v");
#endif

	Console::PrintLogTime(__FUNCTION__, "Attach success");

	if (!FrostbiteFunctions::Initiating())
	{
		Console::Attach("Error");
		Console::PrintLogTime(__FUNCTION__, "Failed find engine proc!\n");
		return false;
	}

	if (!HookManager::pHookManager->DoInitialize())
	{
		Console::Attach("Error");
		Console::PrintLogTime(__FUNCTION__, HookManager::pHookManager->What());
		return false;
	}

	Features::pFeatures = std::make_unique<Features::CFeatures>();

	return true;
}

void MainPayload()
{
	while (!KeyHelper::IsKeyPressed(VK_DELETE))
	{
		MainRoutine();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}

void Shutdown()
{
	FrostbiteFunctions::Input::SetBlockInput(false);
	Console::PrintLogTime(__FUNCTION__, "Exit...\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	FreeLibraryAndExitThread(Vars::pVars->m_CoreVars.hThisModule, EXIT_SUCCESS);
}

void MainHackThread(void* arg)
{
	if (Initiating((HMODULE)arg))  {
		MainPayload();
	}

	Shutdown();
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