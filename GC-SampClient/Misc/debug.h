#pragma once
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define DEBUG_MODE



#ifdef DEBUG_MODE
#define DEBUG(message) std::cout << "[DEBUG] " << message << '\n'
#else
#define DEBUG(message)
#endif
inline FILE* f;
#define ALLOC_CONSOLE() AllocConsole(); freopen_s(&f, "CONOUT$", "w", stdout)
#define FREE_CONSOLE() 	fclose(f); FreeConsole()
#define OUTPUT_ERROR(message) std::cout << "[ERROR] " << message << '\n'

