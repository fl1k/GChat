#pragma once
#include "../IPEndpoint/IPEndpoint.h"
#include "SocketException.h"

namespace GNet
{
#ifdef _WIN32
#define GetSocketError WSAGetLastError
#else
#define GetLastError errno
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