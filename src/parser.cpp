#include "parser.hpp"

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> Parser::parse(const std::string& input)
{
    std::vector<std::string> tokens;

    std::stringstream ss(input);
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}
