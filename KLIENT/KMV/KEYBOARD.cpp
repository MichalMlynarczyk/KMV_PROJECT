// #include <iostream>
// #include <Windows.h>
// #include <thread>
#include <list>
/*
    ** -- KLASA KEYBOARD SENDER -- **

    -> ZBIERANIE DANYCH Z KLAWIATURY     <-
    -> WYSYŁANIE DANYCH Z KLAWIATURY     <-
    -> AKTYWACJA KLAWIATUYU              <-
    -> DEAZAKTYWACJA KLAWIATURY          <-

*/
static std::list<int> keyCodeSave;
// static int keyCodeSave = -1;


class KEYBOARD {
public:

    HHOOK keyboardHook;
    bool stopFlag           = false;                      // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU


    ////////////////////////////////////////////
    ////////////////////////////////////////////

    void _start(){
        _deactivateKeyboard();
    }

    void _stop(){
        stopFlag        = true; 
        _activateKeyboard();
    }

    ~KEYBOARD(){
        std::cout << "KONIEC KEYBOARD" << std::endl;
    }
    ////////////////////////////////////////////
    ////////////////////////////////////////////

    static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        KEYBOARD* pThis = reinterpret_cast<KEYBOARD*>(GetModuleHandle(NULL));
        return pThis->HandleKeyboardHook(nCode, wParam, lParam);
    }

    LRESULT HandleKeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {

        INPUT_LISTENER input;
        input._startupWinsock();
        input._createHintStructure(6100, "192.168.1.23");

        if (nCode >= 0) {

            KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            std::cout << "ACTION: " << kbStruct->vkCode << std::endl;

            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                int flaga = -1;

                for (const auto& element : keyCodeSave) {
                    if (element == kbStruct->vkCode){
                        flaga = 1;
                    }
                }

                if (flaga == -1){

                    std::cout << "KEY DOWN: " << kbStruct->vkCode << std::endl;

                    string key = "Key down: " + std::to_string(kbStruct->vkCode);
                    //std::cout << key << std::endl;
                    input._send(key);

                    keyCodeSave.push_back(kbStruct->vkCode);

                    return 1;
                } 

                return 1;

            }

            else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                std::cout << "KEY UP: " << kbStruct->vkCode << std::endl;

                auto it = keyCodeSave.begin();
                for (; it != keyCodeSave.end(); ++it) {
                    if (*it == static_cast<int>(kbStruct->vkCode)) {
                        break;  // Znaleziono element
                    }
                }

                if (it != keyCodeSave.end()) {
                    keyCodeSave.erase(it);

                    std::cout << "KEY UP: " << kbStruct->vkCode << std::endl;

                    string key = "Key up: " + std::to_string(kbStruct->vkCode);
                    
                    input._send(key);
                }

                // std::cout << "KEY UP: " << kbStruct->vkCode << std::endl;

                // string key = "Key up: " + std::to_string(kbStruct->vkCode);

                // input._send(key);

                return 1;
            }
            else {

                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                // ** POZBYWANIE SIĘ CTRL PRZY WCISKANIU ALTA ** //
                if (kbStruct->vkCode == 162) {
                    return 1;
                }

                int flaga = -1;

                for (const auto& element : keyCodeSave) {
                    if (element == kbStruct->vkCode){
                        flaga = 1;
                    }
                }

                if (flaga == -1){

                    std::cout << "KEY DOWN: " << kbStruct->vkCode << std::endl;

                    string key = "Key down: " + std::to_string(kbStruct->vkCode);
                    
                    input._send(key);

                    keyCodeSave.push_back(kbStruct->vkCode);

                    return 1;
                }

                return 1;
            }

            return 1;
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    void _deactivateKeyboard() {

        // Wyłączenie obsługi klawiatury
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardHookProc, NULL, 0);
        
        MSG msg;
        while (!stopFlag && GetMessage(&msg, NULL, 0, 0)) {
            // Obsługa innych komunikatów, jeśli wymagane
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        UnhookWindowsHookEx(keyboardHook);
    }

    void _activateKeyboard(){
        UnhookWindowsHookEx(keyboardHook);
    }

};

            //if (    keyCode == 162 
            //      || keyCode == 163
            //      || keyCode == 164 
            //      || keyCode == 165
            //      || keyCode == 91
            //      || keyCode == 161
            //      || keyCode == 20
            //      || keyCode == 9
            //      || keyCode == 160)
            // {




            // //////////////////////////
            // //////////////////////////
            // //////////////////////////

            // // LEWY ALT //
            // if (keyCode == 164){
                
            //     INPUT input;

            //     // Zwolnienie klawisza Alt
            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_LMENU;  // Kod klawisza Alt
            //     input.ki.wScan = MapVirtualKey(VK_LMENU, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     // Symulowanie wciśnięcia i zwolnienia klawisza Alt
            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // PRAWY ALT //
            // if (keyCode == 165){
                
            //     INPUT input;

            //     // Zwolnienie klawisza Alt
            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_RMENU;  // Kod klawisza Alt
            //     input.ki.wScan = MapVirtualKey(VK_RMENU, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     // Symulowanie wciśnięcia i zwolnienia klawisza Alt
            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // WINDOWS //
            // if (keyCode == 91){

            //     INPUT input;

            //     // Zwolnienie klawisza Windows
            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_LWIN; 
            //     input.ki.wScan = 0;
            //     input.ki.dwFlags = KEYEVENTF_KEYUP;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     // Symulowanie wciśnięcia i zwolnienia klawisza Windows
            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // LEWY I PRAWY CTRL // ? 
            // if (keyCode == 162 || keyCode == 163){
    
            //     INPUT input;

            //     // Zwolnienie klawisza Ctrl
            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_CONTROL;  // Kod klawisza Ctrl
            //     input.ki.wScan = 0;
            //     input.ki.dwFlags = KEYEVENTF_KEYUP;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     // Symulowanie wciśnięcia i zwolnienia klawisza Ctrl
            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // PRAWY SHIFT //
            // if (keyCode == 161){

            //     INPUT input;

            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_RSHIFT;  
            //     input.ki.wScan = MapVirtualKey(VK_RSHIFT, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // LEWY SHIFT //
            // if (keyCode == 160){

            //     INPUT input;

            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_LSHIFT;  
            //     input.ki.wScan = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // TAB //
            // if (keyCode == 9){

            //     INPUT input;

            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_TAB;  
            //     input.ki.wScan = MapVirtualKey(VK_TAB, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // // CAPSLOCK //
            // if (keyCode == 20){

            //     INPUT input;

            //     input.type = INPUT_KEYBOARD;
            //     input.ki.wVk = VK_CAPITAL;  
            //     input.ki.wScan = MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC);
            //     input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            //     input.ki.time = 0;
            //     input.ki.dwExtraInfo = 0;

            //     SendInput(1, &input, sizeof(INPUT));

            //     return;
            // }

            // //////////////////////////
            // //////////////////////////
            // //////////////////////////