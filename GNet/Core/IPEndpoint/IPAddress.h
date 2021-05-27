#pragma once
#include <vector>
#include <string>
#include "IPVersion.h"

namespace GNet
{
	class IPAddress
	{
	protected:
		std::vector<char> ipBytes;
		std::string ip;
		IPVersion ipVersion;
		IPAddress(IPVersion ipVersion);
		IPAddress(const char* ip, IPVersion ipVersion);
		IPAddress(std::vector<char> ipBytes, const char* ip, IPVersion ipVersion);

	public:
		IPAddress();
		const void* GetIpBytes() const;
		virtual const std::string& ToString() const;
		const IPVersion GetVersion() const;
	};
}