#include "IPv6Address.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#endif

namespace GNet
{
	IPv6Address::IPv6Address() :
		IPAddress(std::vector<char>({ 0,0,0,0,0,0,0,0, 0,0,0,0, 0,0,0,0 }), "::", IPVersion::IPv6)
	{
		this->ipVersion = IPVersion::IPv6;
		this->ip = "::";
		this->ipBytes.resize(IPv6Address::BYTE_SIZE);
		memcpy(this->ipBytes.data(), &in6addr_any, IPv6Address::BYTE_SIZE);
	}

	IPv6Address::IPv6Address(const char* ip) :
		IPAddress(ip, IPVersion::IPv6)
	{
		this->ipBytes.resize(IPv6Address::BYTE_SIZE);
		if (inet_pton(AF_INET6, this->ip.c_str(), this->ipBytes.data()) != 1)
			this->ipVersion = IPVersion::Unknown;
	}

	IPv6Address::IPv6Address(const void* networkByteOrderBytes) :
		IPAddress(IPVersion::IPv6)
	{
		this->ipBytes.resize(IPv6Address::BYTE_SIZE);
		memcpy(this->ipBytes.data(), networkByteOrderBytes, IPv6Address::BYTE_SIZE);
		char ipBuffer[IPv6Address::TEXT_SIZE] = {};
		if (inet_ntop(AF_INET6, this->ipBytes.data(), ipBuffer, IPv6Address::TEXT_SIZE) == 0)
			this->ipVersion = IPVersion::Unknown;
		this->ip = ipBuffer;
	}

	IPv6Address::IPv6Address(const char* ip, const void* networkByteOrderBytes) :
		IPAddress(ip, IPVersion::IPv6)
	{
		this->ipBytes.resize(IPv6Address::BYTE_SIZE);
		memcpy(this->ipBytes.data(), networkByteOrderBytes, IPv6Address::BYTE_SIZE);
	}

	IPv6Address IPv6Address::GetLocalIpAddress()
	{
		return IPv6Address();
	}

	IPv6Address IPv6Address::GetIPFromHostname(const char* hostname)
	{
		addrinfo hints = {};
		hints.ai_family = AF_INET6;
		addrinfo* hostinfo = nullptr;
		if (getaddrinfo(hostname, nullptr, &hints, &hostinfo) == 0)
		{
			char byteBuffer[IPv6Address::BYTE_SIZE] = {};
			sockaddr_in6* sockaddr = reinterpret_cast<sockaddr_in6*>(hostinfo->ai_addr);
			memcpy(byteBuffer, &sockaddr->sin6_addr, IPv6Address::BYTE_SIZE);
			freeaddrinfo(hostinfo);
			return IPv6Address(&byteBuffer);
		}
		return IPv6Address();
	}
}