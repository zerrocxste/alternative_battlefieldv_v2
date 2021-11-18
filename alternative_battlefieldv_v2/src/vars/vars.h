namespace Vars
{
	class CVars
	{
	private:
		struct Engine
		{
			bool m_bRadarActive;
		};
		struct Misc
		{
			bool m_bNameTagsAlwaysVisible;
			bool m_bNoRecoil;
			bool m_bNoSpread;
			bool m_bIncreaseFirerate;
		};
		
		void LoadDefaultVars();
	public:
		CVars();
		~CVars();

		Engine m_EngineVars;
		Misc m_MiscVars;
	};
	
	extern std::unique_ptr<CVars> g_pVars;
}