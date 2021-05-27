#pragma once
#include <vector>

typedef unsigned char BYTE;

class Hook
{
private:
	std::vector<BYTE> bytes;
	BYTE* source;
	BYTE* destination;
	size_t length;
	bool isPlaced;

public:
	Hook();
	Hook(void* src, void* dst, const size_t length);
	bool PlaceCall();
	bool PlaceJump();
	bool Undo();
	bool IsHooked();
	BYTE* PlaceTrampoline();
};