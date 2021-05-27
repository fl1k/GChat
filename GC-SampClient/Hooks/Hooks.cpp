#include "Hooks.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../SDK/SAMP.h"
#include <iostream>

bool Hooks::Initialize()
{
	Hooks::sendMessageHook = Hook(reinterpret_cast<BYTE*>(SAMP::base) + SAMP::FN_SEND_MESSAGE_OFFSET, reinterpret_cast<BYTE*>(Hooks::SendMessage_Hooked), 6U);
	Hooks::SendMessageOriginal = reinterpret_cast<Hooks::fnSendMessage>(Hooks::sendMessageHook.PlaceTrampoline());
	return true;
}

void Hooks::Shutdown()
{
	Hooks::sendMessageHook.Undo();
}
