#include "Hook.h"
#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

Hook::Hook()
{
	this->source = nullptr;
	this->destination = nullptr;
	this->length = 0x0;
	this->bytes = {};
	this->isPlaced = false;
}

Hook::Hook(void* source, void* destination, const size_t length)
{
	this->source = reinterpret_cast<BYTE*>(source);
	this->destination = reinterpret_cast<BYTE*>(destination);
	this->length = length;
	this->bytes.resize(this->length);
	this->isPlaced = false;
}

bool Hook::PlaceCall()
{
	if (this->source == nullptr || this->destination == nullptr)
		return false;
	DWORD currentProtection;
	VirtualProtect(this->source, this->length, PAGE_EXECUTE_READWRITE, &currentProtection);
	memcpy(this->bytes.data(), this->source, this->length);
	memset(this->source, 0x90, this->length);
	uintptr_t relativeAddress = (this->destination - this->source) - 0x5;
	*this->source = 0xE8;
	*reinterpret_cast<uintptr_t*>(this->source + 1) = relativeAddress;
	VirtualProtect(this->source, this->length, currentProtection, &currentProtection);
	this->isPlaced = true;
	return true;
}

bool Hook::PlaceJump()
{
	if (this->source == nullptr || this->destination == nullptr)
		return false;
	DWORD currentProtection;
	VirtualProtect(this->source, this->length, PAGE_EXECUTE_READWRITE, &currentProtection);
	memcpy(this->bytes.data(), this->source, this->length);
	memset(this->source, 0x90, this->length);
	uintptr_t relativeAddress = (this->destination - this->source) - 0x5U;
	*this->source = 0xE9;
	*reinterpret_cast<uintptr_t*>(this->source + 1U) = relativeAddress;
	VirtualProtect(this->source, this->length, currentProtection, &currentProtection);
	this->isPlaced = true;
	return true;
}

BYTE* Hook::PlaceTrampoline()
{
	if (this->source == nullptr || this->destination == nullptr)
		return nullptr;
	BYTE* gateway = reinterpret_cast<BYTE*>(VirtualAlloc(0, length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
	memcpy(gateway, this->source, this->length);
	uintptr_t gatewayRelativeAddr = (this->source - gateway) - 0x5U;
	*(gateway + this->length) = 0xE9;
	*reinterpret_cast<uintptr_t*>(gateway + this->length + 1) = gatewayRelativeAddr;
	PlaceJump();
	return gateway;
}

bool Hook::Undo()
{
	if (this->isPlaced == false)
		return false;

	DWORD oldProtect;
	VirtualProtect(this->source, this->length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(this->source, this->bytes.data(), this->length);
	VirtualProtect(this->source, this->length, oldProtect, &oldProtect);

	this->isPlaced = false;
	return true;
}

bool Hook::IsHooked()
{
	return this->isPlaced;
}