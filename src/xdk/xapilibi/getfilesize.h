#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);

#ifdef __cplusplus
}
#endif
