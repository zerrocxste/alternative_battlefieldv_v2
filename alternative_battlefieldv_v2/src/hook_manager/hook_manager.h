namespace HookManager
{
	class CHookManager
	{
	private:
		bool Hook_sub1420C7C90(bool bDoHook);
		bool Hook_sub140970280(bool bDoHook);
	public:
		CHookManager();
		~CHookManager();

		bool DoInitialize();
		bool DoUninitialize();
	};
	
	extern std::unique_ptr<CHookManager> g_pHookManager;
}