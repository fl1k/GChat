#pragma once
#include <string>
#include <memory>
#include <GNet/Core/Connection/TCPConnection.h>

namespace Client
{
	inline const uint32_t EXCEPTION_COLOR = 0xB80000;
	inline const uint32_t CHAT_COLOR = 0xFFFF00;

	inline bool connected;
	inline std::unique_ptr<GNet::TCPSocket> connection;
	bool Initialize(GNet::IPEndpoint ipEndpoint);
	bool Shutdown();
	bool Send(const char* request);
	void HandleReceive();
}