#pragma once

#include <Windows.h>
#include <optional>

namespace Memory
{
	namespace Internal
	{
		inline int filter(unsigned int code, struct _EXCEPTION_POINTERS * ep)
		{
			return code == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
		}

		template<typename T>
		inline T readMemory(DWORD addr, bool & success)
		{
			__try
			{
				auto val = *(T *)(addr);
				success = true;
				return val;
			}
			__except (filter(GetExceptionCode(), GetExceptionInformation()))
			{
				success = false;
			}

			return T();
		}
	}

	template<typename T>
	inline std::optional<T> readMemory(uintptr_t addr)
	{
		bool success = false;
		auto t = Internal::readMemory<T>(addr, success);

		return success ? std::optional<T>(t) : std::optional<T>();
	}
}