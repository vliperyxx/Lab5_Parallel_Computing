#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
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
    isRunning = true;

    while (isRunning) {
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);

        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Failed to accept client connection: " << WSAGetLastError() << std::endl;
            continue;
        }

        char* clientIp = inet_ntoa(clientAddress.sin_addr);
        int clientPort = ntohs(clientAddress.sin_port);

        std::cout << "Accepted connection from: " << clientIp << ":" << clientPort << std::endl;

        std::thread clientThread(&HttpServer::handleClient, this, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    serverSocket = INVALID_SOCKET;
    WSACleanup();
}

void HttpServer::handleClient(SOCKET clientSocket) {
    const int bufferSize = 4096;
    char buffer[bufferSize];

    int receivedBytes = recv(clientSocket, buffer, bufferSize, 0);
    if (receivedBytes <= 0) {
        closesocket(clientSocket);
        return;
    }

    std::string request(buffer, receivedBytes);

    std::istringstream requestStream(request);
    std::string requestLine;
    std::getline(requestStream, requestLine);

    std::istringstream iss(requestLine);
    std::string method, path, version;
    iss >> method >> path >> version;

    if (method == "GET") {
        std::cout << "Request: " << method << " " << path << " " << version << std::endl;
        sendResponse(clientSocket, path);
    }
    else {
        std::cout << "Unsupported HTTP method: " << method << std::endl;
    }

    closesocket(clientSocket);
}

void HttpServer::sendResponse(SOCKET clientSocket, const std::string& path) {
}