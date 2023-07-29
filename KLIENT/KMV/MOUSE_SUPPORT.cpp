/*
    ** -- KLASA MOUSE SUPPRT -- **

    -> WYŁĄCZAIE MYSZKI                                 <-
    -> ZAWIERA STATYCZNE METODY DO STEROWANIA MYSZKĄ    <-

*/

#include <iostream>

static HHOOK mouseHook;

class MOUSE_SUPPORT {
public:

    int PORT_NUMBER         = 6100;
    string IP_NUMBER        = "192.168.1.23";

    bool stopFlag           = false;                // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU


    //////////////////////////////////////////////
    //////////////////////////////////////////////
    
    void _start(){

        // ** WYŁĄCZNIE MYSZKI ** //
        _deactivateMouse();

    }

    void _stop(){               

        // ** WŁĄCZENIE MYSZKI ** //
        _activeMouse();

        // ** ZAKOŃCZENIE DZIAŁANIA OBIEKTU ** //
        stopFlag = true; 
    }

    ~MOUSE_SUPPORT(){
        //std::cout << "KONIEC MOUSE SUPPORT" << std::endl;
    }

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        MOUSE_SUPPORT* mouseSupport = reinterpret_cast<MOUSE_SUPPORT*>(lParam);
        return mouseSupport->HandleMouseHook(nCode, wParam, lParam);
    }

    LRESULT HandleMouseHook(int nCode, WPARAM wParam, LPARAM lParam) {

        if (nCode >= 0) {
            if (wParam == WM_LBUTTONDOWN) {
             //   _send("MOUSE_INPUT_LEFT_ON");
            }
        }

        if (nCode < 0) {


            return CallNextHookEx(mouseHook, nCode, wParam, lParam);
        }

        return 1;
    }

    static void _deactivateMouse() {

        // ** WYŁACZENIE MYSZKI ** //
        mouseHook = SetWindowsHookEx(WH_MOUSE_LL, &MouseHookProc, NULL, 0);

        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0)) {

            if (msg.message == WM_KEYDOWN) {
                WPARAM wParam = msg.wParam;
                LPARAM lParam = msg.lParam;
                char buffer[256];
                GetKeyNameTextA(lParam, buffer, sizeof(buffer));
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
        
    }

    static void _activeMouse() {
        UnhookWindowsHookEx(mouseHook);
    }
};


