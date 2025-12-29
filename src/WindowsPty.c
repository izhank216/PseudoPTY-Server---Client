#include "WindowsPty.h"
#include <stdlib.h>

WindowsPty* WindowsPty_Create(const char* command, int cols, int rows) {
    WindowsPty* pty = (WindowsPty*)malloc(sizeof(WindowsPty));
    if (!pty) return NULL;
    pty->engine = Engine_Create(command, cols, rows);
    if (!pty->engine) {
        free(pty);
        return NULL;
    }
    return pty;
}

void WindowsPty_Destroy(WindowsPty* pty) {
    if (!pty) return;
    Engine_Destroy(pty->engine);
    free(pty);
}

int WindowsPty_Write(WindowsPty* pty, const char* data, int len) {
    return Engine_Write(pty->engine, data, len);
}

int WindowsPty_Read(WindowsPty* pty, char* buffer, int len) {
    return Engine_Read(pty->engine, buffer, len);
}
