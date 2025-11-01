#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _XINPUT_KEYSTROKE { /* Size=0x8 */
    /* 0x0000 */ WORD VirtualKey;
    /* 0x0002 */ WCHAR Unicode;
    /* 0x0004 */ WORD Flags;
    /* 0x0006 */ BYTE UserIndex;
    /* 0x0007 */ BYTE HidCode;
};
typedef _XINPUT_KEYSTROKE XINPUT_KEYSTROKE;

DWORD XInputGetKeystroke(
    DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE *pKeystroke
);

#ifdef __cplusplus
}
#endif
