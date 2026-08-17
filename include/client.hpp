#pragma once

#include <string>

class Client
{
public:
    explicit Client(int socket_fd);

    void handle();

private:
    int socket_fd_;
    std::string read_buffer_;

    bool send_all(const std::string& response);
};
