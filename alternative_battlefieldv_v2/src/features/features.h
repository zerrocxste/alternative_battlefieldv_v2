namespace Features
{
	class CFeatures
	{
	private:
		std::mutex m_mtxCollectData;
		std::vector<DWORD_PTR> m_vClientSoldierEntityList;
		DWORD_PTR m_pLocalClientSoldierEntity;

		void MainRadarHackWork();
		void LockupRadarHackMutex();
	public:
		CFeatures();
		~CFeatures();

		void DrawEnemyInGameRadar();
		void PatchDrawNameTagsAlwaysVisible(bool bIsEnable);
		void PatchNameTagDrawExtendedInfo(bool bIsEnable);

		void ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity);
		void LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity);
		void NoRecoil(bool bIsEnable);
	};

	extern std::unique_ptr<CFeatures> pFeatures;
}