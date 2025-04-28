#include "HttpServer.h"

int main() {
    HttpServer server;

    if (server.initializeServer() && server.loadPages()) {
        server.startListening();
    }

    return 0;
}