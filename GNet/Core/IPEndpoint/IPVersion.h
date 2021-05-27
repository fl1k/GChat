#pragma once
#include <cstdint>

namespace GNet 
{
	enum class IPVersion : uint8_t
	{
		Unknown,
		IPv4,
		IPv6
	};
}