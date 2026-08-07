#include "server.hpp"
#include <iostream>

int main()
{
    try
    {
        Server server(6379);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}