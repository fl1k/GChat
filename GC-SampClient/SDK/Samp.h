#pragma once
#include <cstdint>

namespace SAMP
{
	inline void* base;

	bool Initialize();
	bool AddMessageToChat(const char* message, size_t color = 0x00FF00U);
}