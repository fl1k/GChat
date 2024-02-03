#pragma once
#include "../IPEndpoint/IPEndpoint.h"
#include "SocketException.h"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#else
#define INVALID_SOCKET -1
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>

#endif

namespace GNet
{
#ifdef _WIN32
#define GetSocketError WSAGetLastError
#define EAGAIN WSAEWOULDBLOCK
#else
#define GetSocketError() strerror(errno)
#define GetLastError() errno
#endif

	class Socket
	{
	protected:
		int32_t handle;
		IPVersion ipVersion;
		Socket();
		Socket(int32_t handle, IPVersion ipVersion);

	public:
		void Close();
		uint32_t GetHandle() const;
		bool IsValid() const;
		bool SetBlocking(int32_t isBlocking);
	};
}