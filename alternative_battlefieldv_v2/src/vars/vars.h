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
			bool m_bNameTagDrawExtendedInfo;
			bool m_bNameTagsAlwaysVisible;
			bool m_bNoRecoil;
			bool m_bIncreaseFireRate;
		};
		struct Backend
		{
			bool m_bUpdatedSoundLButtonReleased;
		};
		void LoadDefaultVars();
	public:
		CVars();
		~CVars();

		Core m_CoreVars;
		Hack m_HackVars;
		Backend m_BackendVars;
	};
	
	extern std::unique_ptr<CVars> pVars;
}