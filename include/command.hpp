#pragma once

#include "database.hpp"

#include <string>
#include <vector>

class Command
{
public:
    explicit Command(Database& db);

    std::string execute(const std::vector<std::string>& tokens);

private:
    Database& db_;
};