#include "Pattern.h"

Pattern::Pattern(HANDLE process, HMODULE module_handle)
{
	MODULEINFO module_info = { NULL };

	GetModuleInformation(process, module_handle, &module_info, sizeof(MODULEINFO));

	moduleBase = reinterpret_cast<DWORD>(module_info.lpBaseOfDll);
	moduleSize = module_info.SizeOfImage;
}

DWORD Pattern::Find(char * pattern, char * mask)
{
	DWORD patternLength = static_cast<DWORD>(strlen(mask));

	for (DWORD i = 0; i < moduleSize - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *reinterpret_cast<char *>((moduleBase + i + j));
		}

		if (found)
		{
			return moduleBase + i;
		}
	}
	return NULL;
}