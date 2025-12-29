#ifndef SERVERPACKETS_H
#define SERVERPACKETS_H

#ifdef SERVERPACKETS_EXPORTS
#define SERVERPACKETS_API __declspec(dllexport)
#else
#define SERVERPACKETS_API __declspec(dllimport)
#endif

#include "WindowsPty.h"

typedef struct {
    WindowsPty* pty;
} ServerPackets_Context;

SERVERPACKETS_API void ServerPackets_Init(ServerPackets_Context* ctx, WindowsPty* pty);
SERVERPACKETS_API void ServerPackets_PtyCommand(ServerPackets_Context* ctx, const char* command, int len, char* outBuffer, int* outLen);

#endif
