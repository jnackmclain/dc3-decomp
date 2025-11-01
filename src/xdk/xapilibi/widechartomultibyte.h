#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int WideCharToMultiByte(
    UINT CodePage,
    DWORD dwFlags,
    LPCWSTR lpWideCharStr,
    int cchWideChar,
    LPSTR lpMultiByteStr,
    int cbMultiByte,
    LPCSTR lpDefaultChar,
    LPBOOL lpUsedDefaultChar
);

#ifdef __cplusplus
}
#endif
