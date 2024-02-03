#include "IPv4Address.h"



namespace GNet
{
	IPv4Address::IPv4Address() :
		IPAddress(std::vector<char>{0, 0, 0, 0}, "0.0.0.0", IPVersion::IPv4)
	{
	}

	IPv4Address::IPv4Address(const char* ip) :
		IPAddress(ip, IPVersion::IPv4)
	{
		this->ipBytes.resize(IPv4Address::BYTE_SIZE);
		if (inet_pton(AF_INET, this->ip.c_str(), this->ipBytes.data()) != 1)
			this->ipVersion = IPVersion::Unknown;
	}

	IPv4Address::IPv4Address(const void* networkByteOrderBytes) :
		IPAddress(IPVersion::IPv4)
	{
		this->ipBytes.resize(IPv4Address::BYTE_SIZE);
		char ipBuffer[IPv4Address::TEXT_SIZE] = {};
		memcpy(this->ipBytes.data(), networkByteOrderBytes, IPv4Address::BYTE_SIZE);
		if (inet_ntop(AF_INET, this->ipBytes.data(), ipBuffer, IPv4Address::TEXT_SIZE) == 0)
			this->ipVersion = IPVersion::Unknown;
		this->ip = ipBuffer;
	}

	IPv4Address::IPv4Address(const char* ip, const void* networkByteOrderBytes) :
		IPAddress(ip, IPVersion::IPv4)
	{
		this->ipBytes.resize(IPv4Address::BYTE_SIZE);
		memcpy(this->ipBytes.data(), networkByteOrderBytes, IPv4Address::BYTE_SIZE);
	}

	IPv4Address IPv4Address::GetLocalIpAddress()
	{
		return IPv4Address();
	}

	IPv4Address IPv4Address::GetIPFromHostname(const char* hostname)
	{
		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* hostinfo = nullptr;
		if (getaddrinfo(hostname, nullptr, &hints, &hostinfo) == 0)
		{
			sockaddr_in* sockaddr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
			uint32_t ipBytes = sockaddr->sin_addr.s_addr;
			freeaddrinfo(hostinfo);
			return IPv4Address(&ipBytes);
		}
		return IPv4Address();
	}
}