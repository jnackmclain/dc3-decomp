#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

DWORD SetFilePointer(
    HANDLE hFile, LONG lDistanceToMove, LONG *lpDistanceToMoveHigh, DWORD dwMoveMethod
);

#ifdef __cplusplus
}
#endif
