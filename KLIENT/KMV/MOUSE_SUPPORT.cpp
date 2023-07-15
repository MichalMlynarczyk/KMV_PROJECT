/*
    ** -- KLASA MOUSE SUPPRT -- **

    -> WYŁĄCZAIE MYSZKI                                 <-
    -> ZAWIERA STATYCZNE METODY DO STEROWANIA MYSZKĄ    <-

*/

#include <iostream>

static HHOOK mouseHook;

class MOUSE_SUPPORT {
public:

    static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        MOUSE_SUPPORT* mouseSupport = reinterpret_cast<MOUSE_SUPPORT*>(lParam);
        return mouseSupport->HandleMouseHook(nCode, wParam, lParam);
    }

    LRESULT HandleMouseHook(int nCode, WPARAM wParam, LPARAM lParam) {

        if (nCode >= 0) {
            if (wParam == WM_LBUTTONDOWN) {
                std::cout << "Left mouse button down" << std::endl;
            }
        }

        if (nCode < 0) {

            std::cout << "FLAGA_1" << std::endl;

            return CallNextHookEx(mouseHook, nCode, wParam, lParam);
        }

        return 1;
    }

    static void _deactivateMouse() {

        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, &MouseHookProc, NULL, 0);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {

            if (msg.message == WM_KEYDOWN) {
                WPARAM wParam = msg.wParam;
                LPARAM lParam = msg.lParam;
                char buffer[256];
                GetKeyNameTextA(lParam, buffer, sizeof(buffer));

                std::cout << "Key Down - Key: " << static_cast<int>(wParam) << ", Name: " << buffer << std::endl;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
    }

    static void _activeMouse() {
        UnhookWindowsHookEx(mouseHook);
    }
};


