#include "SAMP.h"
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool SAMP::Initialize()
{
	SAMP::base = GetModuleHandleA("samp.dll");
	if (SAMP::base == nullptr)
		return false;
	SAMP::chatInfo = *(void**)(reinterpret_cast<uintptr_t>(SAMP::base) + SAMP::ST_CHAT_INFO_OFFSET);
	if (SAMP::chatInfo == nullptr)
		return false;
	SAMP::AddMessageToChatOriginal = reinterpret_cast<fnAddMessageToChat>(reinterpret_cast<uintptr_t>(SAMP::base) + SAMP::FN_ADD_TO_CHAT_OFFSET);
	return true;
}


bool SAMP::AddMessageToChat(const char* message, uint32_t color)
{
	uint32_t chunkPtr = 0;
	while (strlen(message) > SAMP::ADD_TO_CHAT_CHUNK_SIZE + chunkPtr)
	{
		SAMP::AddMessageToChatOriginal(chatInfo, 8, message + chunkPtr, nullptr, color, 0);
		chunkPtr += SAMP::ADD_TO_CHAT_CHUNK_SIZE;
	}
	SAMP::AddMessageToChatOriginal(chatInfo, 8, message + chunkPtr, nullptr, color, 0);
	return true;
}