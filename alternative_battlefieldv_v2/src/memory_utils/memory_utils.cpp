#include "../alternative.h"

namespace memory_utils
{
	static HMODULE base;

	HMODULE get_base()
	{
		if (!base)
			base = GetModuleHandle(NULL);
		return base;
	}

	DWORD_PTR get_base_address()
	{
		return (DWORD_PTR)get_base();
	}

	bool is_valid_ptr(PVOID ptr)
	{
		return (ptr >= (PVOID)0x10000) && (ptr < PTRMAXVAL) && ptr != nullptr && !IsBadReadPtr(ptr, sizeof(ptr));
	}

	char* read_string(std::vector<DWORD_PTR>address)
	{
		return read_pointer<char*>(address);
	}

	wchar_t* read_wstring(std::vector<DWORD_PTR>address)
	{
		return read_pointer<wchar_t*>(address);
	}

	bool write_string(std::vector<DWORD_PTR>address, char* my_value)
	{
		char* ptr = read_pointer<char*>(address);

		if (ptr == NULL)
			return false;

		strcpy(ptr, my_value);

		return true;
	}

	bool write_wstring(std::vector<DWORD_PTR> address, wchar_t* my_value)
	{
		wchar_t* ptr = read_pointer<wchar_t*>(address);

		if (ptr == NULL)
			return false;

		lstrcpyW(ptr, my_value);

		return true;
	}

	DWORD get_module_size(DWORD_PTR address)
	{
		return PIMAGE_NT_HEADERS(address + PIMAGE_DOS_HEADER(address)->e_lfanew)->OptionalHeader.SizeOfImage;
	}

	DWORD_PTR compare_mem(const char* pattern, const char* mask, DWORD_PTR base, DWORD size, const int patternLength, DWORD speed)
	{
		for (DWORD i = 0; i < size - patternLength; i += speed)
		{
			bool found = true;
			for (int j = 0; j < patternLength; j++)
			{
				if (mask[j] == '?')
					continue;

				if (pattern[j] != *(char*)(base + i + j))
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return base + i;
			}
		}

		return NULL;
	}

	DWORD_PTR pattern_scanner_module(HMODULE module, const char* pattern, const char* mask, DWORD scan_speed)
	{
		auto base = (DWORD_PTR)module;
		auto size = get_module_size(base);

		int patternLength = (int)strlen(mask);

		return compare_mem(pattern, mask, base, size, patternLength, scan_speed);
	}

	DWORD_PTR pattern_scanner(
		DWORD_PTR start, DWORD_PTR end,
		const char* pattern, const char* mask,
		DWORD scan_speed,
		DWORD page_prot, DWORD page_state, DWORD page_type)
	{
		auto pattern_length = strlen(mask);

		MEMORY_BASIC_INFORMATION mbi{};
		while (start < end &&
			VirtualQuery((void*)start, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			auto fix_seg = false;
			DWORD_PTR start_seg = (DWORD_PTR)mbi.BaseAddress;
			DWORD_PTR size_seg = mbi.RegionSize;

			if (page_prot != 0 && mbi.Protect != page_prot)
				goto next_seg;

			if (page_state != 0 && mbi.State != page_state)
				goto next_seg;

			if (page_type != 0 && mbi.Type != page_type)
				goto next_seg;

			if (start > start_seg)
			{
				size_seg -= start - start_seg;
				fix_seg = true;
			}

			if (auto compare_result = compare_mem(pattern, mask, start, end, pattern_length, scan_speed))
				return compare_result;

			next_seg:

			fix_seg ? start += size_seg : start = start_seg + size_seg;
		}

		return NULL;
	}

	std::vector<DWORD_PTR> pattern_scanner_vec(
		DWORD_PTR start, DWORD_PTR end,
		const char* pattern, const char* mask,
		DWORD scan_speed,
		DWORD page_prot, DWORD page_state, DWORD page_type)
	{
		std::vector<DWORD_PTR>ret;

		auto pattern_length = strlen(mask);

		MEMORY_BASIC_INFORMATION mbi{};
		while (start < end &&
			VirtualQuery((void*)start, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			auto fix_seg = false;
			DWORD_PTR start_seg = (DWORD_PTR)mbi.BaseAddress;
			DWORD_PTR size_seg = mbi.RegionSize;

			if (page_prot != 0 && mbi.Protect != page_prot)
				goto next_seg;

			if (page_state != 0 && mbi.State != page_state)
				goto next_seg;

			if (page_type != 0 && mbi.Type != page_type)
				goto next_seg;

			if (start > start_seg)
			{
				size_seg -= start - start_seg;
				fix_seg = true;
			}

			if (auto compare_result = compare_mem(pattern, mask, start, end, pattern_length, scan_speed))
				ret.push_back(compare_result);

			next_seg:

			fix_seg ? start += size_seg : start = start_seg + size_seg;
		}

		return ret;
	}

	bool patch_instruction(DWORD_PTR instruction_address, const char* instruction_bytes, int sizeof_instruction_byte)
	{
		DWORD dwOldProtection = NULL;

		if (VirtualProtect((LPVOID)instruction_address, sizeof_instruction_byte, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		{
			memcpy((LPVOID)instruction_address, instruction_bytes, sizeof_instruction_byte);

			if (VirtualProtect((LPVOID)instruction_address, sizeof_instruction_byte, dwOldProtection, &dwOldProtection))
			{
				FlushInstructionCache(GetCurrentProcess(), (LPVOID)instruction_address, sizeof_instruction_byte);
				return true;
			}
		}

		return false;
	}

	bool fill_memory_region(DWORD_PTR instruction_address, int byte, int sizeof_instruction_byte)
	{
		DWORD dwOldProtection = NULL;

		if (VirtualProtect((LPVOID)instruction_address, sizeof_instruction_byte, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		{
			memset((LPVOID)instruction_address, byte, sizeof_instruction_byte);
			if (VirtualProtect((LPVOID)instruction_address, sizeof_instruction_byte, dwOldProtection, &dwOldProtection))
			{
				FlushInstructionCache(GetCurrentProcess(), (LPVOID)instruction_address, sizeof_instruction_byte);
				return true;
			}
		}

		return false;
	}
}


