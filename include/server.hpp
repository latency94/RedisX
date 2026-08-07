#pragma once

class Server {
public:
    explicit Server(int port);
    void start();

private:
    int port_;
    int server_fd_;
};