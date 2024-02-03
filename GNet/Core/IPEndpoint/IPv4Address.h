#pragma once
#include "IPAddress.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#endif
#include <iostream>

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