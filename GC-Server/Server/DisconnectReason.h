#pragma once
#include <cstdint>

enum class DisconnectReason : uint8_t
{
	Disconnect,
	Timeout,
	Kick
};