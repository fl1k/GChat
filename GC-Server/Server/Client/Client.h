#pragma once
#include <GNet/Core/Connection/TCPConnection.h>
#include "ClientFlag.h"

class Client : public GNet::TCPConnection
{
public:
	static const int32_t USERNAME_MIN_LENGTH = 3;
	static const int32_t USERNAME_MAX_LENGTH = 20;
	static const int32_t PASSWORD_MIN_LENGTH = 5;
	static const int32_t PASSWORD_MAX_LENGTH = 32;
	uint64_t id;
	std::string username;
	std::string password;
	std::string lowerUsername; // will be used for comparisons to avoid having to toLower username
	ClientFlag flag;
	bool isLoggedIn;

	static bool IsValidUsername(const std::string& username);
	static bool IsValidPassword(const std::string& password);

	Client(GNet::TCPSocket socket, GNet::IPEndpoint ipEndpoint);
};