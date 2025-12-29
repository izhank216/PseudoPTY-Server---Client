#define ENGINE_EXPORTS
#include "Engine.h"
#include <stdlib.h>
#include <windows.h>

Engine* Engine_Create(const char* command, int cols, int rows) {
    if (!command) return NULL;
    Engine* engine = (Engine*)malloc(sizeof(Engine));
    if (!engine) return NULL;

    HANDLE hPipeInRead, hPipeInWrite, hPipeOutRead, hPipeOutWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hPipeInRead, &hPipeInWrite, &sa, 0)) { free(engine); return NULL; }
    if (!CreatePipe(&hPipeOutRead, &hPipeOutWrite, &sa, 0)) { free(engine); return NULL; }

    COORD size = { (SHORT)cols, (SHORT)rows };
    if (CreatePseudoConsole(size, hPipeInRead, hPipeOutWrite, 0, &engine->hPC) != S_OK) {
        CloseHandle(hPipeInRead); CloseHandle(hPipeInWrite);
        CloseHandle(hPipeOutRead); CloseHandle(hPipeOutWrite);
        free(engine);
        return NULL;
    }

    STARTUPINFOEX si = { sizeof(STARTUPINFOEX) };
    SIZE_T attrSize = 0;
    InitializeProcThreadAttributeList(NULL, 1, 0, &attrSize);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(attrSize);
    InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attrSize);
    UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, engine->hPC, sizeof(HPCON), NULL, NULL);

    PROCESS_INFORMATION pi;
    if (!CreateProcess(NULL, (LPSTR)command, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, (LPSTARTUPINFO)&si, &pi)) {
        DeleteProcThreadAttributeList(si.lpAttributeList);
        free(si.lpAttributeList);
        free(engine);
        return NULL;
    }

    engine->pi = pi;
    engine->hInputWrite = hPipeInWrite;
    engine->hOutputRead = hPipeOutRead;
    CloseHandle(hPipeInRead);
    CloseHandle(hPipeOutWrite);
    DeleteProcThreadAttributeList(si.lpAttributeList);
    free(si.lpAttributeList);
    return engine;
}

void Engine_Destroy(Engine* engine) {
    if (!engine) return;
    CloseHandle(engine->hInputWrite);
    CloseHandle(engine->hOutputRead);
    TerminateProcess(engine->pi.hProcess, 0);
    CloseHandle(engine->pi.hProcess);
    CloseHandle(engine->pi.hThread);
    ClosePseudoConsole(engine->hPC);
    free(engine);
}

int Engine_Write(Engine* engine, const char* data, int len) {
    DWORD written;
    if (!WriteFile(engine->hInputWrite, data, len, &written, NULL)) return -1;
    return (int)written;
}

int Engine_Read(Engine* engine, char* buffer, int len) {
    DWORD read;
    if (!ReadFile(engine->hOutputRead, buffer, len, &read, NULL)) return -1;
    return (int)read;
}
