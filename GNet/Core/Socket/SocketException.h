#pragma once
#include <string>

namespace GNet
{
	class SocketException
	{
	private:
		std::string message;
	public:
		SocketException(const std::string& message) : message(message) {}
		inline const std::string& what() const { return(this->message); };
	};
}