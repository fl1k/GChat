#include "Client.h"

bool Client::IsValidUsername(const std::string& username)
{
	if (username.length() < USERNAME_MIN_LENGTH || username.length() > USERNAME_MAX_LENGTH)
		return false;

	for (size_t i = 0; i < username.length(); i++)
		if (!((username[i] >= 'a' && username[i] <= 'z') || (username[i] >= 'A' && username[i] <= 'Z') || (username[i] >= '0' && username[i] <= '9') 
			|| username[i] == '_'))
			return false;

	return true;
}

bool Client::IsValidPassword(const std::string& password)
{
	if (password.length() < PASSWORD_MIN_LENGTH || password.length() > PASSWORD_MAX_LENGTH)
		return false;

	for (size_t i = 0; i < password.length(); i++)
		if (password[i] < '(' || password[i]> '}')
			return false;
	return true;
}

Client::Client(GNet::TCPSocket socket, GNet::IPEndpoint ipEndpoint) : 
	GNet::TCPConnection(socket, ipEndpoint),
	username(""),
	password(""),
	lowerUsername(""),
	isLoggedIn(false),
	flag(ClientFlag::Default),
	id(0U)
{
}
