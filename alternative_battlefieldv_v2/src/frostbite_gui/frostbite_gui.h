namespace FrostbiteGui
{
	constexpr auto iNameMaxSize = 128;

	class CFrostbiteGui
	{
	public:
		CFrostbiteGui();
		~CFrostbiteGui();

		void EraseWindowData();

		void NewWindow(const char* pszName, std::uint32_t x, std::uint32_t y, std::uint32_t* iCurrentlyTabHovered, float flFontSize, std::uint32_t iSizeX = 0, std::uint32_t iSizeY = 0);
		void EndWindow(__int64 pUnk);
		bool AddCheckbox(__int64 pUnk, const char* pszText, bool* pVarible);
		void AddText(__int64 pUnk, const char* pszText);

		float GetActualWidthLine();

		void RecalcWindowWidth(float flCurrentWidthList);

		void SameLine();
	private:
		int iWindowInnerSpacing = 7;
		int iTitlebarSizeHeight = 18;

		float CalcTextLength(char* szText);
		float CalcSpaceOffset(int iOfs);

		const int m_iItemSpaceSize = 15;

		bool m_bMenuNewPosStarted;
		std::uint32_t m_iStartPosX, m_iStartPosY;
		float m_flBGWidth, m_flBGHeight;
		std::uint32_t m_iNewPosX, m_iNewPosY;
		std::uint32_t m_iCurrentlyTabItemsSize;
		std::uint32_t m_iCurrentlyItemsCount;
		std::uint32_t* m_pMenuCurrentlySelected;
		float m_flFontSize;
		bool m_bReturnIsPressed, m_bReturnIsDowned, m_bReturnIsReleased;
		bool m_bIsSameLine;
		int m_flLastSameLine;
		float m_flMaxWindowWidth;
		char m_szLastItemLabel[3 + 1 + iNameMaxSize];
		char* m_pszWindowTitle;
	};
}



