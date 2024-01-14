#include <vector>
#include "Client.hpp"
#include "Replies.hpp"
#include <algorithm>

#define WHITESPACES " "

std::vector<std::string> parse(std::string input)
{
	std::vector<std::string> args;
	size_t index = 0;
	if (input.empty())
		return args;
	std::replace( input.begin(), input.end(), '\r', ' ');
	std::replace( input.begin(), input.end(), '\t', ' ');
	std::replace( input.begin(), input.end(), '\v', ' ');
	std::replace( input.begin(), input.end(), '\f', ' ');
	while ((index = input.find(WHITESPACES)) != std::string::npos)
	{
		args.push_back(input.substr(0, index));
		input.erase(0, index + 1);
	}
	if (!input.empty())
		args.push_back(input);
	return args;
}


