#include "client.hpp"
#include "parser.hpp"
#include "command.hpp"
#include "database.hpp"

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

static Database database;

Client::Client(int socket_fd)
    : socket_fd_(socket_fd)
{
}

void Client::handle()
{
    Parser parser;
    Command command(database);

    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(socket_fd_, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        std::string input(buffer);

        if (!input.empty() && input.back() == '\n')
            input.pop_back();

        if (!input.empty() && input.back() == '\r')
            input.pop_back();

        auto tokens = parser.parse(input);

        std::string response = command.execute(tokens);

        response += "\n";

        send(socket_fd_,
             response.c_str(),
             response.size(),
             0);
    }

    close(socket_fd_);
}