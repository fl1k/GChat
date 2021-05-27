#include "Socket.h"

namespace GNet
{
	Socket::Socket() :
		handle(0),
		ipVersion(IPVersion::Unknown)
	{
	}

	Socket::Socket(int32_t handle, IPVersion ipVersion) :
		handle(handle),
		ipVersion(ipVersion)
	{
	}

	void Socket::Close()
	{
#ifdef _WIN32
#define close closesocket
#endif
		if (close(this->handle) != 0)
			throw SocketException("GNet::Socket::Close - Failed closing socket. Error: " + std::to_string(GetLastError()));
	}

	uint32_t Socket::GetHandle() const
	{
		return this->handle;
	}

	bool Socket::IsValid() const
	{
		return this->handle != INVALID_SOCKET;
	}

	bool Socket::SetBlocking(int32_t isBlocking)
	{
#ifdef _WIN32
#define ioctl ioctlsocket
#endif
		setsockopt(this->handle, IPPROTO_TCP, TCP_NODELAY, (char*)&isBlocking, sizeof(int32_t)); 
		if (ioctl(this->handle, FIONBIO, reinterpret_cast<u_long*>(&isBlocking)) != 0)
			return true;
		return false;
	}
}