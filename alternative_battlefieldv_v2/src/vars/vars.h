namespace Vars
{
	class CVars
	{
	private:
		struct Core
		{
			HMODULE hThisModule;
		};
		struct Hack
		{
			bool m_bRadarActive;
			bool m_bNameTagsAlwaysVisible;
			bool m_bNameTagDrawExtendedInfo;
			bool m_bNoRecoil;
			bool m_bIncreaseFireRate;
			bool m_bReloadInScope;
		};
		struct Backend
		{
			bool m_bUpdatedSoundLButtonReleased;
		};
		struct Menu
		{
			bool m_bMenuOpened;
		};
		void LoadDefaultVars();
	public:
		CVars();
		~CVars();

		Core m_CoreVars;
		Hack m_HackVars;
		Backend m_BackendVars;
		Menu m_MenuVars;
	};
	
	extern std::unique_ptr<CVars> pVars;
}