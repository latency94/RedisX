#include "resp.hpp"

#include <cstdlib>

static bool read_line(
    const std::string& buffer,
    std::size_t start,
    std::string& line,
    std::size_t& next
)
{
    std::size_t pos = buffer.find("\r\n", start);

    if (pos == std::string::npos)
        return false;

    line = buffer.substr(start, pos - start);
    next = pos + 2;

    return true;
}

bool Resp::parse(
    std::string& buffer,
    std::vector<std::string>& tokens
)
{
    if (buffer.empty())
        return false;

    if (buffer[0] != '*')
        return false;

    std::string line;
    std::size_t pos;

    if (!read_line(buffer, 1, line, pos))
        return false;

    int count = std::stoi(line);

    std::vector<std::string> result;

    for (int i = 0; i < count; ++i)
    {
        if (pos >= buffer.size() || buffer[pos] != '$')
            return false;

        if (!read_line(buffer, pos + 1, line, pos))
            return false;

        int length = std::stoi(line);

        if (pos + length + 2 > buffer.size())
            return false;

        result.push_back(
            buffer.substr(pos, length)
        );

        pos += length;

        if (buffer.substr(pos, 2) != "\r\n")
            return false;

        pos += 2;
    }

    buffer.erase(0, pos);

    tokens = std::move(result);

    return true;
}

std::string Resp::simple_string(
    const std::string& value
)
{
    return "+" + value + "\r\n";
}

std::string Resp::bulk_string(
    const std::string& value
)
{
    return "$" +
           std::to_string(value.size()) +
           "\r\n" +
           value +
           "\r\n";
}

std::string Resp::integer(long long value)
{
    return ":" + std::to_string(value) + "\r\n";
}

std::string Resp::error(
    const std::string& message
)
{
    return "-" + message + "\r\n";
}
