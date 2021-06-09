#pragma once
#include <stdint.h>

#ifdef ERROR
#undef ERROR
#endif

namespace StatusColor
{
	const uint32_t ERROR = 0xC00000;
	const uint32_t SUCCESS = 0x00DD00;
}