#pragma once
#include <Windows.h>
#include <Psapi.h>

class Pattern
{
private:
	DWORD moduleBase;
	DWORD moduleSize;

public:
	Pattern(HANDLE process, HMODULE module_handle);
	DWORD Find(char * pattern, char * mask);
};