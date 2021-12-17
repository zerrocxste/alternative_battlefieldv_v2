namespace HookManager
{
	class CHookManager
	{
	private:
		bool DisableHook(void* Address);
		bool Hook_sub1420C7C90(bool bDoHook);
		bool Hook_sub140970280(bool bDoHook);
		bool Hook_sub14958F0D0(bool bDoHook);
		bool Hook_sub1405C10A0(bool bDoHook);
	public:
		CHookManager();
		~CHookManager();

		bool DoInitialize();
		bool DoUninitialize();
	};
	
	extern std::unique_ptr<CHookManager> pHookManager;
}