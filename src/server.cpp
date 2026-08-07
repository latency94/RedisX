#include "server.hpp"
#include "client.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port)
    : port_(port),
      server_fd_(-1)
{
}

void Server::start()
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd_ < 0)
    {
        std::cerr << "Socket creation failed\n";
        return;
    }

    int opt = 1;

    setsockopt(
        server_fd_,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_,
             (sockaddr*)&address,
             sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd_);
        return;
    }

    if (listen(server_fd_, 5) < 0)
    {
        std::cerr << "Listen failed\n";
        close(server_fd_);
        return;
    }

    std::cout << "=============================\n";
    std::cout << "RedisX Server Started\n";
    std::cout << "Listening on port " << port_ << '\n';
    std::cout << "=============================\n";

    while (true)
    {
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);

        int client_socket = accept(
            server_fd_,
            (sockaddr*)&client_address,
            &client_len
        );

        if (client_socket < 0)
        {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "Client Connected\n";

        Client client(client_socket);
        client.handle();

        std::cout << "Client Disconnected\n";
    }

    close(server_fd_);
}