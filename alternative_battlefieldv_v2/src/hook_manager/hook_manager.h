namespace HookManager
{
	class CHookManager : public IError
	{
	private:
		bool DisableHook(void* Address);
		
		bool Hook_sub1420C7C90();
		bool Hook_sub140970280();
		bool Hook_sub14958F0D0();
		bool Hook_sub1405C10A0();
	public:
		CHookManager();
		~CHookManager();

		bool EnableHook(const char* szPattern, const char* szMask, void* pProxyFunction, void* pGateFunction, std::uintptr_t* pFuncAddress = nullptr, HMODULE hModule = memory_utils::get_base());

		bool DoInitialize();
		bool UnhookAll();

		void ShowError() override;
	};
	
	extern std::unique_ptr<CHookManager> pHookManager;
}