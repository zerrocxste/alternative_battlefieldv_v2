namespace Console
{
	static bool bIsConsoleInitialized = false;

	static void Attach(const char* pszTitle)
	{
		if (!AllocConsole())
			return;

		SetConsoleTitle(pszTitle);

		freopen("conout$", "w", stdout);

		bIsConsoleInitialized = true;
	}

	static void PrintLogTime(const char* pszFunctionName, const char* pszText)
	{
		if (!bIsConsoleInitialized)
			return;

		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%H:%M:%S", timeinfo);

		printf("[%s] %s -> %s\n", buffer, pszFunctionName, pszText);
	}
}