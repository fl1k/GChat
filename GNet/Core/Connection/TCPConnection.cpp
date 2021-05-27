#include "TCPConnection.h"

namespace GNet
{
	TCPConnection::TCPConnection(TCPSocket socket, IPEndpoint ipEndpoint) :
		ipEndpoint(std::move(ipEndpoint)),
		socket(std::move(socket)),
		buffer{}
	{
	}

	std::string TCPConnection::ToString() const
	{
		return this->ipEndpoint.ToString();
	}

	void TCPConnection::Close()
	{
		this->socket.Close();
	}
}