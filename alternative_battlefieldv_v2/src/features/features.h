namespace Features
{
	class CFeatures
	{
	private:
		std::mutex m_mtxCollectData;
		DWORD_PTR m_pLocalClientSoldierEntity;
		std::vector<DWORD_PTR> m_vClientSoldierEntityList;

		void MainRadarHackWork();
		void LockupRadarHackMutex();

		bool m_bMenuNewPosStarted;
		std::uint32_t m_iNewPosX, m_iNewPosY;
		std::uint32_t m_iCurrentlyTabItemsSize;
		std::uint32_t* m_piMenuCurrentlySelected;
		float m_flFontSize;
		bool m_bReturnIsPressed, m_bReturnIsDowned, m_bReturnIsReleased;

		void MenuStartPos(__int64 pUnk, const char* pszName, int x, int y, int iSizeX, int iSizeY, std::uint32_t* iCurrentlyTabHovered, float flFontSize = 1.f);
		void MenuEndPos();
		bool MenuAddTabCheckbox(__int64 pUnk, const char* pszTabName, bool* pVarible);
		
		void DrawMenu(__int64 pUnk);
	public:
		CFeatures();
		~CFeatures();

		void ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity);
		void LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity);

		void DrawEnemyInGameRadar();
		void PatchDrawNameTagsAlwaysVisible(bool bIsEnable);
		void PatchNameTagDrawExtendedInfo(bool bIsEnable);
		void NoRecoil(bool bIsEnable);
		void IncreaseFireRate(bool bIsEnable, float flRate = 0.02f);
		void PatchInScopeReloading(bool bIsEnable);

		void DrawScreen(__int64 pUnk);
	};

	extern std::unique_ptr<CFeatures> pFeatures;
}