#pragma once
#include "IPAddress.h"
#include "IPv4Address.h"
#include "IPv6Address.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2ipdef.h>
#else
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#endif

namespace GNet
{
	class IPEndpoint
	{
	private:
		IPAddress ipAddress;
		uint16_t port;
		std::vector<char> resolvedAddr;

	public:
		IPEndpoint();
		IPEndpoint(IPAddress ipAddress, uint16_t port);
		IPEndpoint(sockaddr& sockaddr);
		const IPAddress& GetIPAddress() const;
		const uint16_t GetPort() const;
		const IPVersion GetVersion() const;
		const sockaddr* GetSockAddr() const;
		const size_t GetSockAddrLength() const;
		std::string ToString() const;
	};

}