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

		void DrawEngineText(__int64 RenderBase, int x, int y, const char* pszText, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a, float flTextSize);
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

		void DrawScreen(__int64 RenderBase);
	};

	extern std::unique_ptr<CFeatures> pFeatures;
}