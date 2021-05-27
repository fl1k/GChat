#pragma once
#include <cstdint>

namespace SAMP
{
	typedef void(__thiscall* fnAddMessageToChat) (void* object, uint32_t messageType, const char* text, const char* prefix, uint32_t textColor, uint32_t prefixColor);

	constexpr uintptr_t FN_SEND_MESSAGE_OFFSET = 0x57F0U;
	constexpr uintptr_t FN_ADD_TO_CHAT_OFFSET = 0x64010U;
	constexpr uintptr_t ST_CHAT_INFO_OFFSET = 0x21A0E4U;

	constexpr size_t ADD_TO_CHAT_CHUNK_SIZE = 86;

	inline void* base;
	inline void* chatInfo;
	inline fnAddMessageToChat AddMessageToChatOriginal;

	bool Initialize();
	bool AddMessageToChat(const char* message, size_t color = 0x00FF00U);
}