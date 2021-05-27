#pragma once
#include <string>

namespace GNet
{
	class PacketException
	{
	private:
		std::string message;
	public:
		PacketException(const std::string& message) : message(message) {}
		inline const std::string& what() const { return(this->message); };
	};
}