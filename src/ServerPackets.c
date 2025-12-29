#define SERVERPACKETS_EXPORTS
#include "ServerPackets.h"

static ServerPackets_Context* g_ctx = NULL;

void ServerPackets_Init(ServerPackets_Context* ctx, WindowsPty* pty) {
    g_ctx = ctx;
    g_ctx->pty = pty;
}

void ServerPackets_PtyCommand(ServerPackets_Context* ctx, const char* command, int len, char* outBuffer, int* outLen) {
    if (!ctx || !command || !outBuffer || !outLen) return;
    Engine_Write(ctx->pty->engine, command, len);
    int readBytes = Engine_Read(ctx->pty->engine, outBuffer, *outLen);
    *outLen = (readBytes > 0) ? readBytes : 0;
}
