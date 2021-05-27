#pragma once
#include "LogReason.h"
#include <iostream>
#include <string>

namespace Logger
{
	void Write(std::string message, Reason reason = Reason::Info);
}