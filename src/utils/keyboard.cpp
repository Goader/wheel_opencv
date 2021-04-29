#include "../../include/utils/keyboard.h"

void press(int code) {
    INPUT pressed;

    pressed.type = INPUT_KEYBOARD;
    pressed.ki.wVk = code;
    pressed.ki.wScan = MapVirtualKey(code, MAPVK_VK_TO_VSC);
    pressed.ki.time = 0;
    pressed.ki.dwFlags = KEYEVENTF_SCANCODE;

    SendInput(1, &pressed, sizeof(INPUT));
}

void release(int code) {
    INPUT released;
    
    released.type = INPUT_KEYBOARD;
    released.ki.wVk = code;
    released.ki.wScan = MapVirtualKey(code, MAPVK_VK_TO_VSC);
    released.ki.time = 0;
    released.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    SendInput(1, &released, sizeof(INPUT));
}
