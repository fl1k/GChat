#pragma once
#include <vector>
#include <string>


class Command
{
private:
	std::string command;
	std::vector<std::string> arguments;

public:
	static const size_t MAX_ARGUMENT_COUNT = 16;
	Command(const std::string& str);

	size_t GetArgumentCount() const;
	const std::string& GetCommand() const;
	const std::string& GetArgument(size_t index) const;
};