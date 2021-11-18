namespace Features
{
	class CFeatures
	{
	private:
		std::mutex m_mtxCollentData;
		std::vector<DWORD_PTR> m_vClientSoldierEntityList;
		DWORD_PTR m_pLocalClientSoldierEntity;
	public:
		CFeatures();
		~CFeatures();

		void DrawEnemyInGameRadar();
		void PatchDrawNameTagsAlwaysVisible(bool bIsEnable);

		void ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity);
		void LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity);
		void NoRecoil(bool bIsEnable);
		void NoSpread(bool bIsEnable);
		void IncreaseFireRate(bool bIsEnable, float flRate = 0.02f);
	};

	extern std::unique_ptr<CFeatures> g_pFeatures;
}