#include "Server.h"
#include "WindowsPty.h"
#include "ServerPackets.h"

#define _WINSOCKAPI_ 
#include <winsock2.h>


#include <windows.h>


#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

static const int DEFAULT_PORT = 5091;

int main(int argc, char** argv) {
    const char* terminal = NULL;
    int port = DEFAULT_PORT;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--terminal") == 0 && i + 1 < argc) {
            terminal = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        }
    }

    if (!terminal) {
        printf("Usage: %s --terminal TERMINAL.exe [--port PORT]\n", argv[0]);
        return 1;
    }

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) return 1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listenSock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) return 1;
    if (listen(listenSock, 1) == SOCKET_ERROR) return 1;

    printf("Server listening on port %d\n", port);

    SOCKET clientSock = accept(listenSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) return 1;

    printf("A client connected to the server.\n");

    WindowsPty* pty = WindowsPty_Create(terminal, 80, 25);
    if (!pty) return 1;

    ServerPackets_Context ctx;
    ServerPackets_Init(&ctx, pty);

    char inBuf[1024];
    char outBuf[8192];

    for (;;) {
        int recvLen = recv(clientSock, inBuf, sizeof(inBuf), 0);
        if (recvLen <= 0) break;

        int outLen = sizeof(outBuf);
        ServerPackets_PtyCommand(&ctx, inBuf, recvLen, outBuf, &outLen);

        if (outLen > 0) {
            send(clientSock, outBuf, outLen, 0);
        }
    }

    WindowsPty_Destroy(pty);
    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
