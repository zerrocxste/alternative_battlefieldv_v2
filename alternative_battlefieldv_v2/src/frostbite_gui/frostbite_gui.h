class CFrostbiteGui
{
public:
	CFrostbiteGui();
	~CFrostbiteGui();

	void MenuStartPos(__int64 pUnk, const char* pszName, int x, int y, int iSizeX, int iSizeY, std::uint32_t* iCurrentlyTabHovered, float flFontSize = 1.f);
	void MenuEndPos();
	bool MenuAddTabCheckbox(__int64 pUnk, const char* pszTabName, bool* pVarible);

private:
	bool m_bMenuNewPosStarted;
	std::uint32_t m_iNewPosX, m_iNewPosY;
	std::uint32_t m_iCurrentlyTabItemsSize;
	std::uint32_t* m_piMenuCurrentlySelected;
	float m_flFontSize;
	bool m_bReturnIsPressed, m_bReturnIsDowned, m_bReturnIsReleased;
};

