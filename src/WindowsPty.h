#ifndef WINDOWSPTY_H
#define WINDOWSPTY_H

#include "Engine.h"

typedef struct {
    Engine* engine;
} WindowsPty;

__declspec(dllexport) WindowsPty* WindowsPty_Create(const char* command, int cols, int rows);
__declspec(dllexport) void WindowsPty_Destroy(WindowsPty* pty);
__declspec(dllexport) int WindowsPty_Write(WindowsPty* pty, const char* data, int len);
__declspec(dllexport) int WindowsPty_Read(WindowsPty* pty, char* buffer, int len);

#endif
