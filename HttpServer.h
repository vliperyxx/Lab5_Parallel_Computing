#pragma once

#include <string>
#include <winsock2.h>

class HttpServer {
public:
    bool initializeServer();
    bool loadPages();
    void startListening();

private:
    void handleClient(SOCKET clientSocket);
    void sendResponse(SOCKET clientSocket, const std::string& path);

    SOCKET serverSocket = INVALID_SOCKET;
    int port = 8080;
    bool isRunning = false;

    std::string indexPageContent;
    std::string secondPageContent;
    std::string badRequestContent;
    std::string errorPageContent;
};

