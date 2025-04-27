#pragma once

#include <string>
#include <winsock2.h>

class HttpServer {
public:
    bool initializeServer();
    void startListening();

private:
    SOCKET serverSocket = INVALID_SOCKET;
    int port = 8080;
};

