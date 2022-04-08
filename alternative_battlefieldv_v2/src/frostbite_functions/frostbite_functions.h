namespace FrostbiteFunctions
{
	namespace Input
	{
		bool SetBlockInput(bool bIsBlock);
	}

	namespace Drawing
	{
		void DrawEngineText(__int64 pUnk, int x, int y, const char* pszText, Color color, float flTextSize);
		void DrawEngineLine(__int64 pUnk, float x, float y, float x1, float y1, Color color);
		void DrawEngineFilledRect(__int64 pUnk, float x, float y, float x1, float y1, Color color);
		void DrawEngineRect(__int64 pUnk, float x, float y, float x1, float y1, Color color);
		int* GetGameScreenResolution();
	}

	bool Initiating();
}