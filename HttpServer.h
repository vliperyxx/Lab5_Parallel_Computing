#pragma once

#include <string>
#include <winsock2.h>

class HttpServer {
public:
    bool initializeServer();
    bool loadPages();
    void startListening();

private:
    SOCKET serverSocket = INVALID_SOCKET;
    int port = 8080;

    std::string indexPageContent;
    std::string secondPageContent;
    std::string badRequestContent;
    std::string errorPageContent;
};

