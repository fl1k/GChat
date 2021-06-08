#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <GNet/Core/Network.h>
#include <thread>
#include "Hooks/Hooks.h"
#include "SDK/SAMP.h"
#include "Client/Client.h"
#include "Config/Config.h"
#include "Misc/debug.h"

void Entry(HMODULE hModule)
{
#ifdef DEBUG_MODE
	ALLOC_CONSOLE();

	// check for game initialization first
	while (!*reinterpret_cast<int *>(0xB6F5F0)) Sleep(50);

#endif	
	if (Config::Load("gc_config.ini") == false)
	{
		Sleep(100);
	}

	if (Config::Load("gc_config.ini") == false)
	{
		SAMP::AddMessageToChat("[ERROR] Failed loading config.ini, check your GTA folder.", 0xC00000);
		FreeLibraryAndExitThread(hModule, 0);
	}

	if (GNet::Network::Initialize() == false)
	{
		SAMP::AddMessageToChat("[ERROR] GNet failed initializing.", 0xC00000);
		GNet::Network::Shutdown();
		FreeLibraryAndExitThread(hModule, 0);
		return;
	}

	if (Hooks::Initialize() == false)
	{
		SAMP::AddMessageToChat("[ERROR] GChat failed placing hooks.", 0xC00000);
		GNet::Network::Shutdown();
		FreeLibraryAndExitThread(hModule, 0);
		return;
	}

	SAMP::AddMessageToChat("GChat initialized.", 0x00FF00);

	GNet::IPAddress ipAddress;
	if (Config::useHostName == true)
		ipAddress = GNet::IPv4Address::GetIPFromHostname(Config::address.c_str());
	else
		ipAddress = GNet::IPv4Address(Config::address.c_str());

	if (Client::Initialize(GNet::IPEndpoint(ipAddress, Config::port)) == true)
	{
		std::thread t(Client::HandleReceive);
		t.detach();
		if (Client::connected == true)
		{
			std::string loginRequest = "/login " + Config::username + " " + Config::password;
			Client::Send(loginRequest.c_str());
		}
	}

	/*
	while (true)
	{
			if (GetAsyncKeyState(VK_END))
				break;
		Sleep(150);
	}

	if (Client::connected == true)
		Client::Send("/logout");

	Sleep(1000);
	SAMP::AddMessageToChat("GChat shut down.");
	Client::Shutdown();
	Hooks::Shutdown();
	GNet::Network::Shutdown();*/
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = nullptr;
		hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Entry, hModule, 0, 0);
		if (hThread)
			CloseHandle(hThread); // Thread so it doesn't block the gamethread
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
