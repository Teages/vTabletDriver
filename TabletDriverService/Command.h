#pragma once

#include "CommandLine.h"
#include <functional>
#include <string>


class Command {
public:

	std::string name;
	std::function<bool(CommandLine*)> callback;

	bool Execute(CommandLine *cmd);

	Command();
	Command(std::string _name, std::function<bool(CommandLine*)> _callback);
	~Command();
};
