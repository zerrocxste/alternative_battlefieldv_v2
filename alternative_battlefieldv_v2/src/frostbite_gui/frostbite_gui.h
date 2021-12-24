namespace FrostbiteGui
{
	enum ITEMS
	{
		CHECKBOX,
		//...
		ITEMS_MAX_SIZE
	};

	constexpr auto iNameMaxSize = 64;

	class CFrostbiteGui
	{
	public:
		CFrostbiteGui();
		~CFrostbiteGui();

		void MenuStartPos(const char* pszName, std::uint32_t x, std::uint32_t y, std::uint32_t* iCurrentlyTabHovered, float flFontSize, std::uint32_t iSizeX = 0, std::uint32_t iSizeY = 0);
		void MenuEndPos(__int64 pUnk);
		bool AddCheckbox(__int64 pUnk, const char* pszTabName, bool* pVarible);

		void SameLine();
	private:
		int iWindowInnerSpacing = 7;
		int iTitlebarSizeSizeY = 18;

		float CalcTextLength(char* szText);
		float CalcSpaceOffset(int iOfs);

		const int m_iItemSpaceSize = 15;

		bool m_bMenuNewPosStarted;
		std::uint32_t m_iStartPosX, m_iStartPosY;
		float m_flBGSizeX, m_flBGSizeY;
		std::uint32_t m_iNewPosX, m_iNewPosY;
		std::uint32_t m_iCurrentlyTabItemsSize;
		std::uint32_t m_iCurrentlyItemsCount;
		std::uint32_t* m_pMenuCurrentlySelected;
		float m_flFontSize;
		bool m_bReturnIsPressed, m_bReturnIsDowned, m_bReturnIsReleased;
		bool m_bIsSameLine;
		int m_flLastSameLine;
		float m_flMaxXLine;
		char m_szLastItemLabel[3 + 1 + iNameMaxSize];
		char* m_pszWindowTitle;
	};
}



