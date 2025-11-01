#include "os/Keyboard.h"
#include "os/HolmesClient.h"
#include "xdk/XAPILIB.h"

namespace {
    int TranslateVK(unsigned short, bool);
}

void KeyboardInit() { KeyboardInitCommon(); }
void KeyboardTerminate() { KeyboardTerminateCommon(); }

void KeyboardPoll() {
    XINPUT_KEYSTROKE keystroke;
    DWORD res = XInputGetKeystroke(0xFF, 2, &keystroke);
    if (res != 0x10D2 && res == 0) {
        if (!(keystroke.Flags & 2)) {
            WCHAR w[2] = { keystroke.Unicode, 0 };
            char c;
            bool shift = keystroke.Flags & 8;
            bool ctrl = keystroke.Flags & 16;
            bool alt = keystroke.Flags & 32;
            WideCharToMultiByte(0, 0, w, 1, &c, 2, nullptr, nullptr);
            int key;
            if (c != '\0' && c >= ' ' && c <= '~') {
                key = c;
            } else {
                key = TranslateVK(keystroke.VirtualKey, shift);
            }
            if (key != 0) {
                KeyboardSendMsg(key, shift, ctrl, alt);
            }
        }
    }
    HolmesClientPollKeyboard();
}
