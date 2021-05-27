#include "Command.h"

Command::Command(const std::string& str)
{
	if (str.length() < 2)
		return;

	this->arguments = {};
	size_t spacePos = str.find(' ');
	if (spacePos == std::string::npos)
		this->command = str.substr(1U);
	else
	{
		this->command = str.substr(1U, spacePos - 1U);
		size_t oldSpacePos = spacePos + 1U;
		while ((spacePos = str.find(' ', oldSpacePos)) != std::string::npos)
		{
			if (this->arguments.size() > MAX_ARGUMENT_COUNT)
				break;
			this->arguments.push_back(str.substr(oldSpacePos, spacePos - oldSpacePos));
			oldSpacePos = spacePos + 1U;
		}
		this->arguments.push_back(str.substr(oldSpacePos));
	}
}

size_t Command::GetArgumentCount() const
{
	return this->arguments.size();
}

const std::string& Command::GetCommand() const
{
	return this->command;
}

const std::string& Command::GetArgument(size_t index) const
{
	return this->arguments[index];
}
