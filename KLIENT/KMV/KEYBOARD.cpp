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

// MOŻE TAK BYĆ, PONIEWAŻ W JEDNEJ CHWILI I TAK MOŻE BYĆ AKTYWNY TYLKO JEDEN OBIEKT KEYBOARD
static string IP_NUMBER;
static int PORT_NUMBER;

class KEYBOARD {
public:

    HHOOK keyboardHook;
    bool stopFlag           = false;                      // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU


    ////////////////////////////////////////////
    ////////////////////////////////////////////

    void _start(string ip, int port){

        IP_NUMBER = ip;
        PORT_NUMBER = port;
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

        if (nCode >= 0) {

            KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                int flaga = -1;

                for (const auto& element : keyCodeSave) {
                    if (element == kbStruct->vkCode){
                        flaga = 1;
                    }
                }

                if (flaga == -1){

                    INPUT_LISTENER input;
                    input._startupWinsock();
                    input._createHintStructure(PORT_NUMBER, IP_NUMBER);

                    string key = "Key down: " + std::to_string(kbStruct->vkCode);

                    input._send(key);

                    keyCodeSave.push_back(kbStruct->vkCode);

                    return 1;
                } 

                return 1;

            }

            else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                auto it = keyCodeSave.begin();
                for (; it != keyCodeSave.end(); ++it) {
                    if (*it == static_cast<int>(kbStruct->vkCode)) {
                        break;  // Znaleziono element
                    }
                }

                if (it != keyCodeSave.end()) {

                    INPUT_LISTENER input;
                    input._startupWinsock();
                    input._createHintStructure(PORT_NUMBER, IP_NUMBER);

                    keyCodeSave.erase(it);

                    string key = "Key up: " + std::to_string(kbStruct->vkCode);
                    
                    input._send(key);
                }

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

                    INPUT_LISTENER input;
                    input._startupWinsock();
                    input._createHintStructure(PORT_NUMBER, IP_NUMBER);

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
