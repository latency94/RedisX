#pragma once

class Client {
public:
    explicit Client(int socket_fd);

    void handle();

private:
    int socket_fd_;
};