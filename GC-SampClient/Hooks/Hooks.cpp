#include "Hooks.h"
#include "../Hook/Pattern.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../SDK/SAMP.h"
#include <iostream>

bool Hooks::Initialize()
{
	Pattern pat(GetCurrentProcess(), reinterpret_cast<HMODULE>(SAMP::base));

	DWORD SMAddr = NULL;

	while (SMAddr == NULL) 
	{
		SMAddr = pat.Find(
			(char *)"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x56\x8B\xB4\x24\x00\x00\x00\x00\x8B\xC6",
			(char *)"xx????xxx????xxxx????xx????xxxxx????xx"
		);
		Sleep(500);
	}
	
	Hooks::sendMessageHook = Hook(reinterpret_cast<void *>(SMAddr), reinterpret_cast<BYTE *>(Hooks::SendMessage_Hooked), 6U);
	Hooks::SendMessageOriginal = reinterpret_cast<Hooks::fnSendMessage>(Hooks::sendMessageHook.PlaceTrampoline());

	return true;
}

void Hooks::Shutdown()
{
	Hooks::sendMessageHook.Undo();
}
