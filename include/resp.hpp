#pragma once

#include <string>
#include <vector>

class Resp
{
public:
    static bool parse(
        std::string& buffer,
        std::vector<std::string>& tokens
    );

    static std::string simple_string(
        const std::string& value
    );

    static std::string bulk_string(
        const std::string& value
    );

    static std::string integer(
        long long value
    );

    static std::string error(
        const std::string& message
    );
};
