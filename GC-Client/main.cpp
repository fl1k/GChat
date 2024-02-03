#include <iostream>
#include <thread>
#include <GNet/Core/Network.h>
#include <GNet/Core/Socket/TCPSocket.h>
#include <GNet/Core/Packet/Packet.h>
#include <GC/Packets/RequestPacket.h>
#include <GC/Packets/ChatMessagePacket.h>
#include <GC/Packets/ServerUpdatePacket.h>
#include "Config/Config.h"

#define OUTPUT_ERR(x) std::cout<< "[ERROR] " << x << "\n"
#define DEBUG(x) std::cout<< "[DEBUG] " << x << "\n"

#ifdef _WIN32
#else
#define Sleep usleep
#endif
std::unique_ptr<GNet::TCPSocket> tcpSocket;

void ReceiveThread()
{
	while (true)
	{
		GNet::Packet packet;
		try
		{
			tcpSocket->Recieve(packet);
			if (packet.GetPacketType() == PacketType::ChatMessage)
			{
				ChatMessagePacket chatMessagePacket(packet);
				std::cout << chatMessagePacket.username << ": " << chatMessagePacket.message << '\n';
			}
			else if (packet.GetPacketType() == PacketType::ServerUpdate)
			{
				ServerUpdatePacket serverUpdatePacket(packet);
				std::cout << serverUpdatePacket.message << '\n';
			}
			else
			{
				OUTPUT_ERR("Bad packet received.");
				return;
			}
		}
		catch (GNet::SocketException exception) { OUTPUT_ERR(exception.what()); return; }

		Sleep(100);
	}
}

int main()
{
	if (Config::Load("config.ini") != true)
	{
		OUTPUT_ERR("Failed loading config.ini, check your folder.");
		system("pause");
		return 0;
	}
	if (GNet::Network::Initialize() == false)
	{
		OUTPUT_ERR("Failed initializing network");
		GNet::Network::Shutdown();
	}
	DEBUG("Successfully initialized network");

	GNet::IPAddress ipAddress;
	if (Config::useHostName == true)
		ipAddress = GNet::IPv4Address::GetIPFromHostname(Config::address.c_str());
	else
		ipAddress = GNet::IPv4Address(Config::address.c_str());

	GNet::IPEndpoint serverIPEndpoint(ipAddress, Config::port);
	tcpSocket = std::make_unique<GNet::TCPSocket>(GNet::IPVersion::IPv4);
	try { tcpSocket->Connect(serverIPEndpoint); }
	catch (GNet::SocketException exception) {
		OUTPUT_ERR(exception.what());
		return 0;
	}
	DEBUG("Successfully connected!");
	char buffer[GNet::Packet::MAX_PACKET_SIZE];
	std::string loginRequest = "/login " + Config::username + " " + Config::password;
	RequestPacket requestPacket(loginRequest);
	try { tcpSocket->Send(requestPacket.GeneratePacket()); }
	catch (GNet::SocketException exception) { OUTPUT_ERR(exception.what()); }
	std::thread t(ReceiveThread);
	t.detach();
	while (true)
	{
		memset(buffer, 0, GNet::Packet::MAX_PACKET_SIZE);
		std::cin.getline(buffer, GNet::Packet::MAX_PACKET_SIZE);
		try
		{
			requestPacket = RequestPacket(buffer);
			try { tcpSocket->Send(requestPacket.GeneratePacket()); }
			catch (GNet::SocketException exception) { OUTPUT_ERR(exception.what()); }
		}
		catch (GNet::PacketException exception) { OUTPUT_ERR(exception.what()); }
	}
}