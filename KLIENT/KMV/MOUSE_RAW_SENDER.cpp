#pragma comment (lib, "ws2_32.lib")

using namespace std;

class MOUSE_RAW_SENDER : public SENDER_TEMP{
    public:

    int DISTANCE            = 0;                          // W JAKIEJ POZYCJI TEORETYCZNIE KURSOR ZNAJDUJE
                                                //      SIĘ NA DRUGIM MONITORZE
    string MOUSE_STATE      = "ON";                       // CZY MYSZKA JEST WŁĄCZONA CZY WYŁĄCZONA
    string DIRECTION;                                     // Z JAKIEJ STRONY WYSYŁAĆ DANE
    bool stopFlag           = false;                      // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU

    INPUT_LISTENER input_sender;  
    KEYBOARD keybord;                      

    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////

    void _start(string PORT, string IP, string DIRECTION){

        this -> DIRECTION = DIRECTION;

        /*
            URUCHAMIANIE WYSYŁANIA DANYCH
        */

        // ** TWORZENIE POŁĄCZENIA UDP DLA INPUT SENDER ** //
        input_sender._startupWinsock();
        input_sender._createHintStructure(stoi(PORT) + 1000, IP);

        // ** ZATRZYMYWANIE KLAWIATURY ** //
        std::thread keyboardThread([&]() {
        keybord._start(IP, stoi(PORT) + 1000);});

        keyboardThread.detach(); 


        // ** TWORZENIE POŁĄCZENIA UDP DLA RAW SENDER ** //
        _startupWinsock();
        _createHintStructure(stoi(PORT), IP);   // PRZYGOTOWYWANIE PORTU
        _getMouseRawInformation();              // WYSYŁANIE DANYCH
        _shutdown();
    } 

    void _stop() {

        // ** ZATRZYMANIE KEYBOARD ** //
        keybord._stop();

        // ** ZATRZYMANIE INPUT SENDER ** //
        input_sender._shutdown();
        input_sender._stop();

        stopFlag = true;                        // ZATRZYMANIE
    }

    ~MOUSE_RAW_SENDER(){
        //std::cout << "KONIEC MOUSE RAW SENDER" << std::endl; 
    }

    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////

    void _sendAs(int * VECTOR){
        /*
            WYSYŁA DANE W ZALEŻNOŚCI CZY MYSZKA PRZEKROCZYŁA 
                LEWĄ STRONĘ EKRANU CZY PRAWĄ
        */

        if (DIRECTION == "LEFT"){

            POINT cursorPosition;
            GetCursorPos(&cursorPosition);      // Pobranie aktualnej pozycji myszki

            if (cursorPosition.x == 0 && DISTANCE <= 0){
                DISTANCE += VECTOR[0];

                if (MOUSE_STATE == "ON"){

                    // ** TWORZENIE OBIEKTU MOUSE_SUPPORT ** //
                    MOUSE_SUPPORT mouseSupport;
                    std::thread thread(&MOUSE_SUPPORT::_deactivateMouse);
                    thread.detach();
                    MOUSE_STATE = "OFF";
                }

                string data = std::to_string(VECTOR[0]) + " " + std::to_string(VECTOR[1]);

                _send(data);                                // WYSYŁANIE DANYCH NA SERWER

            } else{

                if (MOUSE_STATE == "OFF"){
                    MOUSE_SUPPORT::_activeMouse();          // WŁĄCZENIE MYSZKI
                    MOUSE_STATE = "ON";                     // USTAWIENIE AKTUALNEGO STANU MYSZKI
                    DISTANCE = 0;                           // ZRESETOWANIE DISTANCE
                }

                _stop();                                     // USUNIĘCIE WĄTKU

            }
        }
        else if (DIRECTION == "RIGHT"){

            POINT cursorPosition;
            GetCursorPos(&cursorPosition);      // Pobranie aktualnej pozycji myszki

            int MAX_SCREEN_WIDTH = GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1;

            if (cursorPosition.x == MAX_SCREEN_WIDTH && DISTANCE >= 0){
                DISTANCE += VECTOR[0];

                if (MOUSE_STATE == "ON"){
                    std::thread thread(&MOUSE_SUPPORT::_deactivateMouse);
                    thread.detach();
                    MOUSE_STATE = "OFF";
                }

                string data = std::to_string(VECTOR[0]) + " " + std::to_string(VECTOR[1]);

                _send(data);                                // WYSYŁANIE DANYCH NA SERWER

            } else{

                if (MOUSE_STATE == "OFF"){
                    MOUSE_SUPPORT::_activeMouse();          // WŁĄCZENIE MYSZKI
                    MOUSE_STATE = "ON";                     // USTAWIENIE AKTUALNEGO STANU MYSZKI
                    DISTANCE = 0;                           // ZRESETOWANIE DISTANCE
                }

                _stop();                                     // USUNIĘCIE WĄTKU

            }
        }
    }

    LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
        switch (message)
        {
            case WM_INPUT:
            {
                UINT bufferSize;
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
                BYTE *buffer = new BYTE[bufferSize];
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &bufferSize, sizeof(RAWINPUTHEADER));

                RAWINPUT *raw = (RAWINPUT*)buffer;

                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    int mX = raw->data.mouse.lLastX;
                    int mY = raw->data.mouse.lLastY;


                    // ** WCIŚNIĘCIE LEWEGO PRZYCISKU MYSZKI ** //
                    if ((raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != 0) {
                        input_sender._send("MOUSE_INPUT_LEFT_ON");
                    }
                    else if ((raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) != 0) {
                        input_sender._send("MOUSE_INPUT_LEFT_OFF");
                    }

                    // ** WCIŚNIĘCIE PRAWEGO PRZYCISKU MYSZKI ** //
                    if ((raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) != 0) {
                        input_sender._send("MOUSE_INPUT_RIGHT_ON");
                    }
                    else if ((raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) != 0) {
                        input_sender._send("MOUSE_INPUT_RIGHT_OFF");
                    }

                    if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL){
					    int x = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
                        string msg = "SCROLL " + std::to_string(x);
                        input_sender._send(msg);
                    }


                    int tab [2];
                    tab[0] = mX;
                    tab[1] = mY;

                    _sendAs(tab);

                }

                delete[] buffer;
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    static LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
        MOUSE_RAW_SENDER* pThis;

        if (message == WM_NCCREATE) {
            pThis = static_cast<MOUSE_RAW_SENDER*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
            SetLastError(0);
            if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
                if (GetLastError() != 0) {
                    return FALSE;
                }
            }
        }
        else {
            pThis = reinterpret_cast<MOUSE_RAW_SENDER*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (pThis) {
            return pThis->WindowProc(hWnd, message, wParam, lParam);
        }
        else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    void _getMouseRawInformation(){
        // Register the window class
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProcStatic;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = "RawInputWindowClass";

        RegisterClassEx(&wc);

        // Create the message-only window
        HWND hWnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            "",
            0,
            0, 0, 0, 0,
            HWND_MESSAGE,
            NULL,
            GetModuleHandle(NULL),
            this // Pass the current instance of MOUSE_RAW_SENDER as the window's user data
        );

        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01; // Generowanie danych dotyczących myszy
        rid.usUsage = 0x02;     // Generowanie danych dotyczących ruchu myszy
        rid.dwFlags = RIDEV_INPUTSINK;
        rid.hwndTarget = hWnd;
        RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));

        MSG msg;
        while (!stopFlag && GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    }


};