#pragma once
#include "../win_types.h"
#include "createdirectory.h"

#ifdef __cplusplus
extern "C" {
#endif

HANDLE CreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);

#ifdef __cplusplus
}
#endif
