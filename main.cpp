#include "HttpServer.h"

int main() {
    HttpServer server;

    if (server.initializeServer()) {
        server.startListening();
    }

    return 0;
}