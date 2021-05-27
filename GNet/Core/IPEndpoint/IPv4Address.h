#pragma once
#include "IPAddress.h"

namespace GNet
{
	class IPv4Address : public IPAddress
	{
	private:
		IPv4Address();

	public:
		static const uint32_t BYTE_SIZE = 4;
		static const uint32_t TEXT_SIZE = 16;
		IPv4Address(const char* ip);
		IPv4Address(const void* networkByteOrderBytes);
		IPv4Address(const char* ip, const void* networkByteOrderBytes);
		static IPv4Address GetLocalIpAddress();
		static IPv4Address GetIPFromHostname(const char* hostname);
	};
}