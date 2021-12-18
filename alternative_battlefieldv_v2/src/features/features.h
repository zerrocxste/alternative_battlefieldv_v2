namespace Features
{
	class CFeatures
	{
	private:
		std::mutex m_mtxCollectData;
		DWORD_PTR m_pLocalClientSoldierEntity;
		std::vector<DWORD_PTR> m_vClientSoldierEntityList;

		bool m_bMenuNewPosStarted;
		std::uint32_t m_iNewPosX, m_iNewPosY;
		std::uint32_t m_iCurrentlyTabItemsSize;
		std::uint32_t* m_pMenuCurrentlySelected;
		bool m_bReturnIsPressed;

		void MainRadarHackWork();
		void LockupRadarHackMutex();

		void DrawEngineText(__int64 pUnk, int x, int y, const char* pszText, Color color, float flTextSize);

		void MenuStartPos(int x, int y, std::uint32_t* iCurrentlyTabHovered);
		void MenuEndPos();
		bool MenuAddTabCheckbox(__int64 pUnk, const char* pszTabName, bool* pVarible);
		
		void DrawMenu(__int64 pUnk);
	public:
		CFeatures();
		~CFeatures();

		void DrawEnemyInGameRadar();
		void PatchDrawNameTagsAlwaysVisible(bool bIsEnable);
		void PatchNameTagDrawExtendedInfo(bool bIsEnable);

		void ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity);
		void LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity);
		void NoRecoil(bool bIsEnable);
		void IncreaseFireRate(bool bIsEnable, float flRate = 0.02f);
		void PatchInScopeReloading(bool bIsEnable);

		void DrawScreen(__int64 pUnk);
	};

	extern std::unique_ptr<CFeatures> pFeatures;
}