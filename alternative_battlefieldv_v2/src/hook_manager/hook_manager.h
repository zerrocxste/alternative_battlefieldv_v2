namespace HookManager
{
	class CHookManager
	{
	private:
		std::vector<void*> m_vFunctionsAddressessList;

		void* FindFunctionMainModule(const char* pszPattern, const char* pszMask);

		bool DisableHook(void* Address);
		
		bool Hook_sub1420C7C90();
		bool Hook_sub140970280();
		bool Hook_sub14958F0D0();
		bool Hook_sub1405C10A0();
	public:
		CHookManager();
		~CHookManager();

		bool DoInitialize();
		bool UnhookAll();
	};
	
	extern std::unique_ptr<CHookManager> pHookManager;
}