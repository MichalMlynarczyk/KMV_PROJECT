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


//KEYBOARD* g_pKeyboardInstance = nullptr;


class KEYBOARD {
public:

 static KEYBOARD* g_pKeyboardInstance;

    HHOOK keyboardHook;
    bool stopFlag           = false;                      // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU
    INPUT_LISTENER input_;


    ////////////////////////////////////////////
    ////////////////////////////////////////////

    void _start(string ip, int port){

        IP_NUMBER = ip;
        PORT_NUMBER = port;

        input_._startupWinsock();
        input_._createHintStructure(PORT_NUMBER, IP_NUMBER);

        _deactivateKeyboard();
    }

    void _stop(){
        stopFlag        = true; 
        _activateKeyboard();
    }

    ~KEYBOARD(){
        // std::cout << "KONIEC KEYBOARD" << std::endl;
    }
    ////////////////////////////////////////////
    ////////////////////////////////////////////

   // LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
   //     std::cout << "OK" << std::endl;
        // HMODULE hModule = GetModuleHandle(NULL);
        // //KEYBOARD* pThis = reinterpret_cast<KEYBOARD*>(GetModuleHandle(NULL));
        // KEYBOARD* pThis = reinterpret_cast<KEYBOARD*>(lParam)->lpCreateParams);
        // return pThis->HandleKeyboardHook(nCode, wParam, lParam);
    //}

    static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) { 
        if (nCode >= 0 && g_pKeyboardInstance) {
            return g_pKeyboardInstance->HandleKeyboardHook(nCode, wParam, lParam);
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    LRESULT HandleKeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {

        if (nCode >= 0) {

            KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

                //KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                int flaga = -1;

                for (const auto& element : keyCodeSave) {
                    if (element == kbStruct->vkCode){
                        flaga = 1;
                    }
                }


                if (flaga == -1){

                    string key;

                    if (kbStruct->vkCode == 163){
                        key = "Key down: 162";
                    } else {
                        key = "Key down: " + std::to_string(kbStruct->vkCode);
                    }

                    input_._send(key);

                    keyCodeSave.push_back(kbStruct->vkCode);

                    return 1;
                } 

                return 1;

            }

            else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

                //KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                auto it = keyCodeSave.begin();
                for (; it != keyCodeSave.end(); ++it) {
                    if (*it == static_cast<int>(kbStruct->vkCode)) {
                        break;  // Znaleziono element
                    }
                }

                if (it != keyCodeSave.end()) {

                    keyCodeSave.erase(it);

                }

                string key;

                if (kbStruct->vkCode == 163){
                    key = "Key up: 162";
                } else {
                    key = "Key up: " + std::to_string(kbStruct->vkCode);
                }
                
                input_._send(key);

                return 1;
            }
            else {

                //KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                // ** POZBYWANIE SIĘ CTRL PRZY WCISKANIU ALTA ** //
                if (kbStruct->vkCode == 162 || kbStruct->vkCode == 163) {
                    return 1;
                }

                int flaga = -1;

                for (const auto& element : keyCodeSave) {
                    if (element == kbStruct->vkCode){
                        flaga = 1;
                    }
                }

                if (flaga == -1){

                    string key;

                    if (kbStruct->vkCode == 163){
                        key = "Key down: 162";
                    } else {
                        key = "Key down: " + std::to_string(kbStruct->vkCode);
                    }
                    
                    input_._send(key);
                    
                    input_._send(key);

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
        g_pKeyboardInstance = this;
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);
        
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

KEYBOARD* KEYBOARD::g_pKeyboardInstance = nullptr; 