namespace Vars
{
	class CVars
	{
	private:
		struct Hack
		{
			bool m_bRadarActive;
			bool m_bNameTagDrawExtendedInfo;
			bool m_bNameTagsAlwaysVisible;
			bool m_bNoRecoil;
		};
		struct Global
		{
			HWND hWndGame;
		};

		void LoadDefaultVars();
	public:
		CVars();
		~CVars();

		Hack m_HackVars;
		Global m_GlobalVars;
	};
	
	extern std::unique_ptr<CVars> pVars;
}