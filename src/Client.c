#include "Client.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

static const int DEFAULT_PORT = 5091;

int main(int argc, char** argv) {
    const char* host = "127.0.0.1";
    int port = DEFAULT_PORT;

    if (argc > 1) host = argv[1];
    if (argc > 2) port = atoi(argv[2]);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return 1;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) != 0) return 1;

    printf("Connected to server %s:%d\n", host, port);

    char input[1024];
    char output[8192];

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = 0;

        send(sock, input, (int)strlen(input), 0);

        int outLen = recv(sock, output, sizeof(output)-1, 0);
        if (outLen <= 0) break;

        output[outLen] = 0;
        printf("%s", output);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
