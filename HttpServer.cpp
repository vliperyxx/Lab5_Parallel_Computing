#include <iostream>
#include <fstream>
#include <sstream>
#include "HttpServer.h"

#pragma comment(lib, "ws2_32.lib")

bool HttpServer::initializeServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed" << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cout << "Socket binding failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool HttpServer::loadPages() {
    std::ifstream indexFile("pages/index.html");
    if (!indexFile) {
        std::cout << "Failed to open index.html" << std::endl;
        return false;
    }
    std::ostringstream ssIndex;
    ssIndex << indexFile.rdbuf();
    indexPageContent = ssIndex.str();

    std::ifstream secondPageFile("pages/page2.html");
    if (!secondPageFile) {
        std::cout << "Failed to open page2.html" << std::endl;
        return false;
    }
    std::ostringstream ssSecond;
    ssSecond << secondPageFile.rdbuf();
    secondPageContent = ssSecond.str();

    badRequestContent = "<html><head><title>400 Bad Request</title></head><body><h1>400 - Bad Request</h1></body></html>";
    errorPageContent = "<html><head><title>404 Not Found</title></head><body><h1>404 - Page Not Found</h1></body></html>";

    return true;
}

void HttpServer::startListening() {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listening socket failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server started listening on port " << port << std::endl;

    closesocket(serverSocket);
    serverSocket = INVALID_SOCKET;
    WSACleanup();
}
