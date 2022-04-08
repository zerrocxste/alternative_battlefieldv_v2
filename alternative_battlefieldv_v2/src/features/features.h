namespace Features
{
	class CFeatures
	{
	private:
		DWORD_PTR m_pLocalClientSoldierEntity;
		std::vector<DWORD_PTR> m_vClientSoldierEntityList;
		CRITICAL_SECTION m_csCollectData;
		std::unique_ptr<FrostbiteGui::CFrostbiteGui> m_pFrostbiteGui;

		std::uint64_t PatchAddressVisibleCheck;
		std::uint64_t PatchAddressWriteVaribles;
		std::uint64_t PatchAddressSmokeCheck;
		std::uint64_t PatchAddressExtendedNametag;
		std::uint64_t PatchAddressScopeReloading;
		std::uint64_t PatchAddressWeaponPitch;
		std::uint64_t PatchAddressWeaponYaw;
		std::uint64_t PatchAddressFirerate;

		void MainRadarHackWork();
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