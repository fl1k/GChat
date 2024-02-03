#pragma once
#include <string>
#include <cstdint>

namespace Config
{
	inline bool useHostName;
	inline std::string address;
	inline uint16_t port;
	inline std::string username;
	inline std::string password;
	bool Load(std::string fileName);
};