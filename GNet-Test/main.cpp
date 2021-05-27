#include <GNet/Core/Network.h>
#include <GNet/Core/IPEndpoint/IPEndpoint.h>
#include <GNet/Core/IPEndpoint/IPv4Address.h>
#include <GNet/Core/IPEndpoint/IPv6Address.h>
#include <GNet/Core/Socket/TCPSocket.h>
#include <iostream>

int main()
{
	if (GNet::Network::Initialize())
	{
		GNet::IPAddress ip = GNet::IPv4Address::GetIPFromHostname("google.com");
		std::cout << ip.ToString() << '\n';
		GNet::IPEndpoint ipEndpoint(ip, 80);
		GNet::TCPSocket socket(GNet::IPVersion::IPv4);
		try { socket.Connect(ipEndpoint); }
		catch (GNet::SocketException& ex) { std::cout << ex.what() << '\n';  return 0; }
		std::cout << "Connected! " << '\n';
		const char http[] = "GET / HTTP/1.1\r\n\r\n";
		char buffer[40000] = {};
		try { socket.SendAll(http, sizeof(http)); }
		catch (GNet::SocketException& ex) { std::cout << ex.what(); return 0; }
		try { socket.ReceiveAll(buffer, 40000); }
		catch (GNet::SocketException& ex) { std::cout << ex.what(); return 0; }
		std::cout << buffer << '\n';
		try { socket.Close(); }
		catch (GNet::SocketException& ex) { std::cout << ex.what() << '\n'; return 0; }
	}
	GNet::Network::Shutdown();
}