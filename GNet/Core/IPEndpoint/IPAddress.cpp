#include "IPAddress.h"


namespace GNet
{
	IPAddress::IPAddress(IPVersion ipVersion) :
		ipVersion(ipVersion)
	{ 
	}

	IPAddress::IPAddress(const char* ip, IPVersion ipVersion) :
		ipBytes{},
		ip(ip),
		ipVersion(ipVersion)
	{ 
	}

	IPAddress::IPAddress(std::vector<char> ipBytes, const char* ip, IPVersion ipVersion) :
		ipBytes(std::move(ipBytes)),
		ip(ip),
		ipVersion(ipVersion)
	{
	}


	IPAddress::IPAddress() :
		ipVersion(IPVersion::Unknown),
		ip(std::string()),
		ipBytes{}
	{
	}

	const void* IPAddress::GetIpBytes() const
	{
		return this->ipBytes.data();
	}

	const std::string& IPAddress::ToString() const
	{
		return this->ip;
	}
	const IPVersion IPAddress::GetVersion() const
	{
		return this->ipVersion;
	}
}