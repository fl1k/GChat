#include "TCPSocket.h"
#include <iostream>

namespace GNet
{
	TCPSocket::TCPSocket() :
		Socket(INVALID_SOCKET, IPVersion::Unknown)
	{
	}

	TCPSocket::TCPSocket(int32_t handle, IPVersion ipVersion) :
		Socket(handle, ipVersion)
	{
	}

	TCPSocket::TCPSocket(IPVersion ipVersion, int32_t protocol)
	{
		this->ipVersion = ipVersion;
		if (this->ipVersion == IPVersion::IPv4)
		{
			this->handle = socket(AF_INET, SOCK_STREAM, protocol);
			if (this->handle == SOCKET_ERROR)
				throw SocketException("GNet::TCPSocket::TCPSocket - socket() failed" + GetSocketError());

		}
		else if (this->ipVersion == IPVersion::IPv6)
		{
			this->handle = socket(AF_INET6, SOCK_STREAM, protocol);
			if (this->handle == SOCKET_ERROR)
				throw SocketException("GNet::TCPSocket::TCPSocket - socket() failed" + GetSocketError());
		}
		else
			throw SocketException("GNet::TCPSocket::TCPSocket - Invalid IPEndpoint");

		if (this->handle == INVALID_SOCKET)
			throw SocketException("GNet::TCPSocket::TCPSocket - socket() failed.");
	}

	void TCPSocket::Connect(const IPEndpoint& ipEndpoint)
	{
		if (this->ipVersion != ipEndpoint.GetVersion())
			throw SocketException("GNet::TCPSocket::Connect - Provided ipEndpoint that doesn't match sockets IPVersion.");
		if (connect(this->handle, ipEndpoint.GetSockAddr(), ipEndpoint.GetSockAddrLength()) != 0)
			throw SocketException("GNet::TCPSocket::Connect - Failed connecting. Error: " + std::to_string(GetSocketError()));
	}

	void TCPSocket::Bind(const IPEndpoint& ipEndpoint)
	{
		if (this->ipVersion != ipEndpoint.GetVersion())
			throw SocketException("GNet::TCPSocket::Bind - Provided ipEndpoint that doesn't match sockets IPVersion.");
		if (bind(this->handle, ipEndpoint.GetSockAddr(), ipEndpoint.GetSockAddrLength()) != 0)
			throw SocketException("GNet::TCPSocket::Bind - Failed binding the IPEndPoint. Error: " + std::to_string(GetSocketError()));
	}

	void TCPSocket::Listen(const IPEndpoint& ipEndpoint, int32_t backLog)
	{
		if (this->ipVersion != ipEndpoint.GetVersion())
			throw SocketException("GNet::TCPSocket::Connect - Provided ipEndpoint that doesn't match sockets IPVersion.");
		if (listen(this->handle, backLog) == SOCKET_ERROR)
			throw SocketException("GNet::TCPSocket::Listen - Failed listening on socket. Error: " + std::to_string(GetSocketError()));
	}

	TCPSocket TCPSocket::Accept()
	{
		uint32_t handle = accept(this->handle, nullptr, nullptr);
		if (handle != INVALID_SOCKET)
			return TCPSocket(handle, this->ipVersion);
		return TCPSocket();
	}

	TCPSocket TCPSocket::Accept(IPEndpoint& ipEndpoint)
	{
		int32_t acceptedHandle = INVALID_SOCKET;
		if (this->ipVersion == IPVersion::IPv4)
		{
			sockaddr_in addr;
			int32_t structSize = sizeof(sockaddr_in);
			acceptedHandle = accept(this->handle, reinterpret_cast<sockaddr*>(&addr), &structSize);
			ipEndpoint = IPEndpoint(*reinterpret_cast<sockaddr*>(&addr));
		}
		if (this->ipVersion == IPVersion::IPv6)
		{
			sockaddr_in6 addr;
			int32_t structSize = sizeof(sockaddr_in6);
			acceptedHandle = accept(this->handle, reinterpret_cast<sockaddr*>(&addr), &structSize);
			ipEndpoint = IPEndpoint(*reinterpret_cast<sockaddr*>(&addr));
		}
		if (this->handle != INVALID_SOCKET)
			return TCPSocket(acceptedHandle, this->ipVersion);
		return TCPSocket();
	}

	void TCPSocket::Send(const void* buffer, size_t bufferSize, int32_t& bytesSent, int32_t flags)
	{
		if ((bytesSent = send(this->handle, reinterpret_cast<const char*>(buffer), bufferSize, flags)) == SOCKET_ERROR)
			throw SocketException("GNet::TCPSocket::Send - Failed sending on socket. Error: " + std::to_string(GetSocketError()));
	}

	void TCPSocket::SendAll(const void* buffer, size_t bufferSize, int32_t flags)
	{
		int32_t totalBytesSent = 0;
		int32_t bytesSent = 0;
		while (true)
		{
			this->Send(reinterpret_cast<const char*>(buffer) + totalBytesSent, bufferSize - totalBytesSent, bytesSent, flags);
			totalBytesSent += bytesSent;
			if (totalBytesSent == bufferSize)
				return;
		}
	}

	void TCPSocket::Receive(void* buffer, size_t bufferSize, int32_t& bytesRecieved, int32_t flags)
	{
		if ((bytesRecieved = recv(this->handle, reinterpret_cast<char*>(buffer), bufferSize, flags)) == SOCKET_ERROR)
			throw SocketException("GNet::TCPSocket::Receive - Failed receiving on socket. Error: " + std::to_string(GetSocketError()));
	}

	void TCPSocket::ReceiveAll(void* buffer, size_t bufferSize, int32_t flags)
	{
		int32_t totalBytesReceived = 0;
		int32_t bytesRecieved = 0;
		while (true)
		{
			this->Receive(reinterpret_cast<char*>(buffer) + totalBytesReceived, bufferSize - totalBytesReceived, bytesRecieved, flags);
			totalBytesReceived += bytesRecieved;
			if (totalBytesReceived == bufferSize)
				return;
		}
	}

	void TCPSocket::Send(const Packet& packet, int32_t flags)
	{
		uint16_t packetSize = htons(static_cast<uint16_t>(packet.GetSize()));
		this->SendAll(&packetSize, sizeof(uint16_t), flags);
		this->SendAll(packet.Data(), packet.GetSize(), flags);
	}

	void TCPSocket::Recieve(Packet& packet, int32_t flags)
	{
		uint16_t packetSize;
		this->ReceiveAll(&packetSize, sizeof(uint16_t), flags);
		packetSize = ntohs(packetSize);
		packet.Resize(packetSize);
		this->ReceiveAll(packet.Buffer(), packetSize, flags);
	}
}