#include "client.hpp"
#include "parser.hpp"
#include "command.hpp"
#include "database.hpp"

#include <cerrno>
#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>

static Database database;

Client::Client(int socket_fd)
    : socket_fd_(socket_fd)
{
}

bool Client::send_all(const std::string& response)
{
    std::size_t total_sent = 0;

    while (total_sent < response.size())
    {
        ssize_t bytes_sent = send(
            socket_fd_,
            response.data() + total_sent,
            response.size() - total_sent,
            0
        );

        if (bytes_sent < 0)
        {
            if (errno == EINTR)
                continue;

            std::cerr << "send failed: "
                      << std::strerror(errno)
                      << '\n';

            return false;
        }

        if (bytes_sent == 0)
            return false;

        total_sent += static_cast<std::size_t>(bytes_sent);
    }

    return true;
}

void Client::handle()
{
    Parser parser;
    Command command(database);

    char buffer[4096];

    while (true)
    {
        ssize_t bytes_received = recv(
            socket_fd_,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytes_received == 0)
            break;

        if (bytes_received < 0)
        {
            if (errno == EINTR)
                continue;

            std::cerr << "recv failed: "
                      << std::strerror(errno)
                      << '\n';

            break;
        }

        read_buffer_.append(
            buffer,
            static_cast<std::size_t>(bytes_received)
        );

        while (true)
        {
            std::size_t newline_pos =
                read_buffer_.find('\n');

            if (newline_pos == std::string::npos)
                break;

            std::string input =
                read_buffer_.substr(0, newline_pos);

            read_buffer_.erase(
                0,
                newline_pos + 1
            );

            if (!input.empty() && input.back() == '\r')
                input.pop_back();

            auto tokens = parser.parse(input);

            std::string response =
                command.execute(tokens);

            response += "\n";

            if (!send_all(response))
                return;
        }
    }

    close(socket_fd_);
}
