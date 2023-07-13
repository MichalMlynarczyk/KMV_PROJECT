/*
    ** -- KLASA MOUSE SUPPRT -- **

    -> WYŁĄCZAIE MYSZKI                                 <-
    -> ZAWIERA STATYCZNE METODY DO STEROWANIA MYSZKĄ    <-

*/

static HHOOK mouseHook;

class MOUSE_SUPPORT {
public:

    static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        MOUSE_SUPPORT* mouseSupport = reinterpret_cast<MOUSE_SUPPORT*>(lParam);
        return mouseSupport->HandleMouseHook(nCode, wParam, lParam);
    }

    LRESULT HandleMouseHook(int nCode, WPARAM wParam, LPARAM lParam) {

        if (nCode < 0) {
            return CallNextHookEx(mouseHook, nCode, wParam, lParam);
        }

        return 1;
    }

    static void _deactivateMouse() {

        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, &MouseHookProc, NULL, 0);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    static void _activeMouse() {
        UnhookWindowsHookEx(mouseHook);
    }
};


