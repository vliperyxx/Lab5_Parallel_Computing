#pragma once

#include <string>
#include <winsock2.h>

class HttpServer {
public:
    bool initializeServer();

private:
    SOCKET serverSocket = INVALID_SOCKET;
    int port = 8080;
};

