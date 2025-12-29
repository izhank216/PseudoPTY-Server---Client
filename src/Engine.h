#ifndef ENGINE_H
#define ENGINE_H

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <windows.h>

typedef struct {
    HANDLE hInputWrite;
    HANDLE hOutputRead;
    PROCESS_INFORMATION pi;
    HPCON hPC;
} Engine;

ENGINE_API Engine* Engine_Create(const char* command, int cols, int rows);
ENGINE_API void Engine_Destroy(Engine* engine);
ENGINE_API int Engine_Write(Engine* engine, const char* data, int len);
ENGINE_API int Engine_Read(Engine* engine, char* buffer, int len);

#endif
