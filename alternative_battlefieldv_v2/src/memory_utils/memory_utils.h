//memory utils
//author: zerrocxste

namespace memory_utils
{
#ifdef _WIN64
#define PTRMAXVAL ((PVOID)0x000F000000000000)
#elif _WIN32
#define PTRMAXVAL ((PVOID)0xFFF00000)
#endif

	extern bool is_valid_ptr(PVOID ptr);

	extern HMODULE get_base();

	extern DWORD_PTR get_base_address();

	extern DWORD get_module_size(DWORD_PTR address);

	extern DWORD_PTR pattern_scanner_module(HMODULE module, const char* pattern, const char* mask, DWORD scan_speed = 0x1);

	DWORD_PTR pattern_scanner(
		DWORD_PTR start, DWORD_PTR end,
		const char* pattern, const char* mask,
		DWORD scan_speed,
		DWORD page_prot = PAGE_EXECUTE_READ, DWORD page_state = MEM_COMMIT, DWORD page_type = MEM_PRIVATE);

	std::vector<DWORD_PTR> pattern_scanner_vec(
		DWORD_PTR start, DWORD_PTR end,
		const char* pattern, const char* mask,
		DWORD scan_speed,
		DWORD page_prot = PAGE_EXECUTE_READ, DWORD page_state = MEM_COMMIT, DWORD page_type = MEM_PRIVATE);

	template<class T>
	T read_pointer(std::vector<DWORD_PTR> list)
	{
		if (list.empty())
			return NULL;

		DWORD_PTR ptr = list.front();

		for (INT_PTR i = 0; i < list.size(); i++)
		{
			if (!is_valid_ptr((LPVOID)ptr))
				break;

			if (i < (INT_PTR)(list.size() - 2))
				ptr = *(DWORD_PTR*)(ptr + list.at(i + 1));
			else
				return list.size() == 1 ? (T)ptr : (T)(ptr + list.back());
		}

		return NULL;
	}

	template<class T>
	T read_value(std::vector<DWORD_PTR> list)
	{
		T* ptr = read_pointer<T*>(list);

		if (ptr == NULL)
			return T();

		return *(T*)ptr;
	}

	template<class T>
	bool write(std::vector<DWORD_PTR> list, T my_value)
	{
		T* ptr = read_pointer<T*>(list);

		if (ptr == NULL)
			return false;

		*ptr = my_value;

		return true;
	}

	extern char* read_string(std::vector<DWORD_PTR> address);
	extern wchar_t* read_wstring(std::vector<DWORD_PTR>address);

	extern bool write_string(std::vector<DWORD_PTR> address, char* my_value);
	extern bool write_wstring(std::vector<DWORD_PTR> address, wchar_t* my_value);

	extern bool patch_instruction(DWORD_PTR instruction_address, const char* instruction_bytes, int sizeof_instruction_byte);
	extern bool fill_memory_region(DWORD_PTR instruction_address, int byte, int sizeof_instruction_byte);
}