#pragma once
#include <cstdint>

namespace SAMP
{

	inline void* base;

	bool Initialize();
	bool AddMessageToChat(const char* message, uint32_t color = 0x00FF00U);
}