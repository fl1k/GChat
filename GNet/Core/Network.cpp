#include "Network.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#else

#endif

namespace GNet
{
	namespace Network
	{
		bool Initialize()
		{
#ifdef _WIN32
			WSAData wsaData;
			WORD wsaVersion = MAKEWORD(2, 2);
			if (WSAStartup(wsaVersion, &wsaData) != 0)
				return false;
			if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
				return false;
#endif
			return true;
		}

		void Shutdown()
		{
#ifdef _WIN32
			WSACleanup();
#endif
		}
	}
}