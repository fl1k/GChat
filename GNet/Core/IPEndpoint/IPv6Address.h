#pragma once
#include "IPAddress.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

namespace GNet
{
	class IPv6Address : public IPAddress
	{
	private:
		IPv6Address();

	public:
		static const uint32_t BYTE_SIZE = 16;
		static const uint32_t TEXT_SIZE = 46;
		IPv6Address(const char* ip);
		IPv6Address(const void* networkByteOrderBytes);
		IPv6Address(const char* ip, const void* networkByteOrderBytes);
		static IPv6Address GetLocalIpAddress();
		static IPv6Address GetIPFromHostname(const char* hostname);
	};
}
