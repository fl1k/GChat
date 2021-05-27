#include "Logger.h"
#include <string.h>

namespace Logger
{
	void Write(std::string message, Reason reason)
	{
		time_t t = time(NULL);
		tm time;
		localtime_s(&time, &t);
		std::string log;
		log.resize(21);

		//[06/19/2015 10:35:50]
		sprintf_s(&log[0], 22, "[%02d/%02d/%d %02d:%02d:%02d]", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900, time.tm_hour, time.tm_min, time.tm_sec);
		switch (reason)
		{
		case Reason::Error:
			log += " [ERROR] ";
			break;
		case Reason::Info:
			log += " [INFO] ";
			break;
		case Reason::Warning:
			log += " [WARNING] ";
			break;
		case Reason::Fatal:
			log += " [FATAL] ";
			break;
		default:
			return;
		}
		log += message;
		std::cout << log << "\n";
	}
}
