#pragma once
#include <cstdint>
#include "Memory/Hook.h"
#include "Client/Client.h"

namespace Hooks
{
	typedef char(__stdcall* fnSendMessage) (const char* message);

	inline fnSendMessage SendMessageOriginal;


	inline Hook sendMessageHook;

	char __stdcall SendMessage_Hooked(const char* message);

	bool Initialize();
	void Shutdown();
}