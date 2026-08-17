#include "command.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

Command::Command(Database& db)
    : db_(db)
{
}


std::string Command::execute(const std::vector<std::string>& tokens)
{
    if (tokens.empty())
        return "ERROR";

    const std::string& cmd = tokens[0];
    
        if (cmd == "PING")
    {
        return "PONG";
    }

    if (cmd == "SET")
    {
        if (tokens.size() != 3)
            return "ERROR";

        db_.set(tokens[1], tokens[2]);
        return "OK";
    }

    if (cmd == "GET")
    {
        if (tokens.size() != 2)
            return "ERROR";

        std::string value;

        if (db_.get(tokens[1], value))
            return value;

        return "(nil)";
    }

    if (cmd == "DEL")
    {
        if (tokens.size() != 2)
            return "ERROR";

        return db_.del(tokens[1]) ? "1" : "0";
    }

    if (cmd == "EXISTS")
    {
        if (tokens.size() != 2)
            return "ERROR";

        return db_.exists(tokens[1]) ? "1" : "0";
    }

    

    return "Unknown Command";
}