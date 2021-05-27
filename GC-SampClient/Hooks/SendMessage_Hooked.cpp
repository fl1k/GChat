#include "Hooks.h"

char __stdcall Hooks::SendMessage_Hooked(const char* message)
{
	if (strlen(message) < 1U)
		return Hooks::SendMessageOriginal(message);
	if (message[0] == '.')
	{
		if (strlen(message) > 5U && message[1] == '/' && message[2] == 's' && message[3] == 'a' && message[4] == 'y' && message[5] == ' ')
			return Hooks::SendMessageOriginal(message + 6U);
		if (strlen(message) > 1U)
			Client::Send(message + 1U);
		return 0x4;

	}
	return Hooks::SendMessageOriginal(message);
}