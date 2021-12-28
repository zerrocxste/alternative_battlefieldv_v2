namespace Console
{
	static bool bIsConsoleInitialized = false;

	static void Attach(const char* pszTitle)
	{
		if (bIsConsoleInitialized || !AllocConsole())
			return;

		SetConsoleTitle(pszTitle);

		freopen("conout$", "w", stdout);

		bIsConsoleInitialized = true;
	}

	static void PrintLogTime(const char* pszFunctionName, const char* pszText, ...)
	{
		if (!bIsConsoleInitialized)
			return;

		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%H:%M:%S", timeinfo);

		char buf[1024 * 3 + 1] = { 0 };
		va_list va;
		va_start(va, pszText);
		vsprintf(buf, pszText, va);
		va_end(va);

		printf("[%s] %s() -> %s\n", buffer, pszFunctionName, buf);
	}
}