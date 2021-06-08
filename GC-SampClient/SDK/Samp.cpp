#include "SAMP.h"
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>
#include "../Hook/Pattern.h"

bool SAMP::Initialize()
{
	SAMP::base = GetModuleHandleA("samp.dll");
	return true;
}


bool SAMP::AddMessageToChat(const char * message, size_t color)
{
	Pattern pat(GetCurrentProcess(), reinterpret_cast<HMODULE>(SAMP::base));

	std::ostringstream convColor;
	convColor << std::hex << color;

	std::string textMsg = "{" + convColor.str() + "}" + message;
	const char * cText = textMsg.c_str();

	auto ACMAddr = pat.Find(
		(char *)"\x8B\x15\x00\x00\x00\x00\x68\x00\x00\x00\x00\x52\xE8\x00\x00\x00\x00\x83\xC4\x08\x5F\x5E",
		(char *)"xx????x????xx????xxxxx"
	);

	if (ACMAddr) {
		DWORD dwCallAddr = *(DWORD *)(ACMAddr + 0xD) + ACMAddr + 0xD + 0x4;
		DWORD dwInfo = *(DWORD *)(ACMAddr + 0x2);

		__asm mov edx, dword ptr[dwInfo]
		__asm mov eax, [edx]
		__asm push cText
		__asm push eax
		__asm call dwCallAddr
		__asm add esp, 8
	}
	else {
		// Error handling maybe?
		return false;
	}

	return true;
}