#include <iostream>
#include <Windows.h>

int main() {
    Sleep(4000);
    std::cout << "Get ready!" << std::endl;
    Sleep(1000);

    for (int i = 0; i < 3000; i++) {
        std::cout << i << std::endl;
        INPUT pressed, released;

        pressed.type = INPUT_KEYBOARD;
        pressed.ki.wVk = 0x57;  // 'W' key
        pressed.ki.wScan = MapVirtualKey(0x57, MAPVK_VK_TO_VSC);
        pressed.ki.time = 0;
        pressed.ki.dwFlags = KEYEVENTF_SCANCODE;

        released.type = INPUT_KEYBOARD;
        released.ki.wVk = 0x57;  // 'W' key
        released.ki.wScan = MapVirtualKey(0x57, MAPVK_VK_TO_VSC);
        released.ki.time = 0;
        released.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

        SendInput(1, &pressed, sizeof(INPUT));
        Sleep(100);
        if (i < 140) {
            SendInput(1, &released, sizeof(INPUT));
            Sleep(100 / (1 + i / 20));
        }
    }

    INPUT released;
    released.type = INPUT_KEYBOARD;
    released.ki.wVk = 0x57;  // 'W' key
    released.ki.wScan = MapVirtualKey(0x57, MAPVK_VK_TO_VSC);
    released.ki.time = 0;
    released.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &released, sizeof(INPUT));

    return 0;
}
