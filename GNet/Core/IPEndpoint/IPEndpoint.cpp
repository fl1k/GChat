#include "IPEndpoint.h"

namespace GNet
{
	IPEndpoint::IPEndpoint() :
		ipAddress(),
		port(0U),
		resolvedAddr{}
	{
	}

	IPEndpoint::IPEndpoint(IPAddress ipAddress, uint16_t port) :
		ipAddress(std::move(ipAddress)),
		port(port),
		resolvedAddr{}
	{
		if (this->ipAddress.GetVersion() == IPVersion::IPv4)
		{
			this->resolvedAddr.resize(sizeof(sockaddr_in));
			sockaddr_in* saddrIn = reinterpret_cast<sockaddr_in*>(this->resolvedAddr.data());
			saddrIn->sin_family = AF_INET;
			saddrIn->sin_port = htons(this->port);
			memcpy(&(saddrIn->sin_addr), this->ipAddress.GetIpBytes(), GNet::IPv4Address::BYTE_SIZE);
			return;
		}
		if (this->ipAddress.GetVersion() == IPVersion::IPv6)
		{
			this->resolvedAddr.resize(sizeof(sockaddr_in6));
			sockaddr_in6* saddrIn = reinterpret_cast<sockaddr_in6*>(this->resolvedAddr.data());
			saddrIn->sin6_family = AF_INET6;
			saddrIn->sin6_port = htons(this->port);
			memcpy(&saddrIn->sin6_addr, this->ipAddress.GetIpBytes(), GNet::IPv6Address::BYTE_SIZE);
			return;
		}
	}

	IPEndpoint::IPEndpoint(sockaddr& sockaddr) :
		ipAddress(),
		port(0U),
		resolvedAddr{}
	{
		if (sockaddr.sa_family == AF_INET) //IPv4
		{
			sockaddr_in* saddrIn = reinterpret_cast<sockaddr_in*>(&sockaddr);
			this->port = ntohs(saddrIn->sin_port);
			this->ipAddress = IPv4Address(&saddrIn->sin_addr.s_addr);
		}
		if (sockaddr.sa_family == AF_INET6)
		{
			sockaddr_in6* saddrIn = reinterpret_cast<sockaddr_in6*>(&sockaddr);
			this->port = ntohs(saddrIn->sin6_port);
			this->ipAddress = IPv6Address(&saddrIn->sin6_addr);
		}
	}

	const IPAddress& IPEndpoint::GetIPAddress() const
	{
		return this->ipAddress;
	}

	const uint16_t IPEndpoint::GetPort() const
	{
		return this->port;
	}

	const IPVersion IPEndpoint::GetVersion() const
	{
		return this->ipAddress.GetVersion();
	}

	const sockaddr* IPEndpoint::GetSockAddr() const
	{
		return reinterpret_cast<const sockaddr*>(this->resolvedAddr.data());
	}

	const size_t IPEndpoint::GetSockAddrLength() const
	{
		if (this->ipAddress.GetVersion() == IPVersion::IPv4)
			return sizeof(sockaddr_in);
		if (this->ipAddress.GetVersion() == IPVersion::IPv6)
			return sizeof(sockaddr_in6);
		return 0U;
	}

	std::string IPEndpoint::ToString() const
	{
		return this->ipAddress.ToString() + ":" + std::to_string(this->port);
	}
}