
/*
    ** -- KLASA INPUT RECEIVER -- **

    -> ODBIERANIE DANYCH O MYSZCZE I KLAWIATURZE <-

*/

class INPUT_RECEIVER : public RECEIVER{

    public:

    int INPUT_RECEIVER_PORT_NUMBER;

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void _start(int PORT_NUMBER){

        this -> INPUT_RECEIVER_PORT_NUMBER = PORT_NUMBER;

        _receiveData(PORT_NUMBER);                 // NASŁUCHIWANIE NA DANYM PORCIE

    }

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    int _getKeyCode(char * buf){

        int *tab = new int[2];
        string keyCodeString = "";
        int keyCode;

        for(int i = 0; i < strlen(buf); i++){
            if (std::isdigit(buf[i])) {
                keyCodeString += buf[i];
            }
        }

        keyCode = std::stoi(keyCodeString);

        return keyCode;
    }

    int _getScrollPos(char * buf){
        
        int *tab = new int[2];
        string keyCodeString = "";
        int keyCode;

        for(int i = 0; i < strlen(buf); i++){
            if (std::isdigit(buf[i])) {
                keyCodeString += buf[i];
            } else if (buf[i] == '-'){
                keyCodeString += buf[i];
            }
        }

        keyCode = std::stoi(keyCodeString);

        return keyCode;
    }

    float _getMonitorProportion(char * buf){

        int *tab = new int[2];
        string keyCodeString = "";
        float keyCode;

        for(int i = 0; i < strlen(buf); i++){
            if (std::isdigit(buf[i])) {
                keyCodeString += buf[i];
            } else if (buf[i] == '.'){
                keyCodeString += buf[i];
            }
        }

        keyCode = std::stof(keyCodeString);

        return keyCode;
    }

    INPUT input;

    void _receivedData(char *buf) override {

        /// ****       MOUSE POSITION        **** ///
        if (std::strncmp(buf, "MP:", 3) == 0){
            float monitor_proportion = _getMonitorProportion(buf);

            POINT currentMousePosition;
            GetCursorPos(&currentMousePosition);         // POBRANIE I ZAPISANIE OBECNEJ POZYCJI MYSZKI

            POINT newMousePosition;
            int monitorHeight = GetSystemMetrics(SM_CYSCREEN);              // WYSOKOŚĆ MONITORA

            float new_height_float = static_cast<float>(monitorHeight) * monitor_proportion;
            int new_height_int = static_cast<int>(new_height_float);

            newMousePosition.y = new_height_int;

            if (INPUT_RECEIVER_PORT_NUMBER == 6100)
                newMousePosition.x = GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1;        
            else if (INPUT_RECEIVER_PORT_NUMBER == 6300)
                newMousePosition.x = 0; 
            

            SetCursorPos(newMousePosition.x, newMousePosition.y);           // PRZESUNIĘCIE MYSZKI
        }

        /// ****           SCROLL              **** ///
        if (std::strncmp(buf, "SCROLL", 6) == 0) {
            int mouseVector = _getScrollPos(buf) * 120;                 // POBRANIE KODU KLAWISZA

            input.type = INPUT_MOUSE;
            input.mi.dx = 0;
            input.mi.dy = 0;
            input.mi.mouseData = mouseVector; // 120 = scroll up, -120 = scroll down
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.time = 0;
            input.mi.dwExtraInfo = 0;

            // Simulate scroll movement
            SendInput(1, &input, sizeof(INPUT));

        }

        /// ****            KEY UP              **** ///

        if (std::strncmp(buf, "Key up", 6) == 0) {

            int keyCode = _getKeyCode(buf);                 // POBRANIE KODU KLAWISZA

            // ** LEWY ALT ** //
            if (keyCode == 164){  

                INPUT input;

                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LMENU;  
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                SendInput(1, &input, sizeof(INPUT));

                return;
            }

            // ** PRAWY ALT ** //
            if (keyCode == 165){  

                INPUT input;

                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_RMENU;  
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                SendInput(1, &input, sizeof(INPUT));

                return;
            }

            // ** WINDOWS ** //
            if (keyCode == 91){

                INPUT input;

                // Zwolnienie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LWIN;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko zwolnienia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            // ** LEWY CTRL ** //
            if (keyCode == 162){
    
                INPUT input;

                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_CONTROL;  // Kod klawisza Ctrl
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie wciśnięcia i zwolnienia klawisza Ctrl
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            // ** LEWY CTRL ** //
            if (keyCode == 162){
    
                INPUT input;

                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_CONTROL;  // Kod klawisza Ctrl
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie wciśnięcia i zwolnienia klawisza Ctrl
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            if (keyCode == 163){
    
                INPUT input;

                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_CONTROL;  // Kod klawisza Ctrl
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie wciśnięcia i zwolnienia klawisza Ctrl
                SendInput(1, &input, sizeof(INPUT));


                return;
            }


            // PRAWY SHIFT //
            if (keyCode == 161){

                INPUT input;

                // Zwolnienie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_RSHIFT;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko zwolnienia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            // LEWY SHIFT //
            if (keyCode == 160){

                INPUT input;

                // Zwolnienie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LSHIFT;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko zwolnienia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            // ** RESZTA PRZYCISKÓW ** //

            keybd_event(keyCode, 0, 0, 0);
        }

        /// ****           KEY DOWN             **** ///

        if (std::strncmp(buf, "Key down", 8) == 0) {    

            int keyCode = _getKeyCode(buf);                 // POBRANIE INFORMACJI O KODZIE KLAWISZA

            //      ***     LEWY CTRL    ***      //
            //      ***     PRAWY CTRL    ***      //
            if (    keyCode == 162 
             //    || keyCode == 163
                 )
            {
                // SYMULACJA CIĄGŁĘGO WCIŚNIĘCIA DANEGO PRZYCISKU

                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(keyCode, 0, 0, 0);
            
                return;
            }

            if (  //  keyCode == 162 
              keyCode == 163
                 )
            {
                // SYMULACJA CIĄGŁĘGO WCIŚNIĘCIA DANEGO PRZYCISKU

                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(keyCode, 0, 0, 0);
            
                return;
            }

            // ** PRAWY SHIFT ** //
            if (keyCode == 161){ 
                INPUT input;

                // Wciśnięcie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_RSHIFT;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko wciśnięcia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));
            
                return;
            }

            // ** LEWY SHIFT ** //
            if (keyCode == 160){ 
                INPUT input;

                // Wciśnięcie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LSHIFT;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko wciśnięcia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));
            
                return;
            }

            // ** LEWY ALT ** //
            if (keyCode == 164){ 
                INPUT input;

                // Wciśnięcie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LMENU;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko wciśnięcia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));
            
                return;
            }

            // ** PRAWY ALT ** //
            if (keyCode == 165){ 

                INPUT input;

                // Wciśnięcie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_RMENU;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko wciśnięcia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));
            
                return;
            }

            // ** WINDOWS ** //
            if ( keyCode == 91 ){

                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(keyCode, 0, 0, 0);
            
                return;
            }

            // ** RESZTA PRZYCISKÓW ** //
                    
            keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);    // NACIŚNIĘCIE KLAWISZA
        }

        /// ****        OBSŁUGA MYSZKI        **** ///

        if (std::strcmp(buf, "MOUSE_INPUT_LEFT_ON") == 0 ){
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        }

        if (std::strcmp(buf, "MOUSE_INPUT_LEFT_OFF") == 0 ){
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }

        if (std::strcmp(buf, "MOUSE_INPUT_RIGHT_ON") == 0 ){
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }

        if (std::strcmp(buf, "MOUSE_INPUT_RIGHT_OFF") == 0 ){
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }

    }

};

