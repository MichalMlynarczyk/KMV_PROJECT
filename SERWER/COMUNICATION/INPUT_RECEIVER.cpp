
/*
    ** -- KLASA INPUT RECEIVER -- **

    -> ODBIERANIE DANYCH O MYSZCZE I KLAWIATURZE <-

*/

class INPUT_RECEIVER : public RECEIVER{

    public:

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void _start(int PORT_NUMBER){

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

    INPUT input;


    void _receivedData(char *buf) override {

        /// ****            KEY UP              **** ///

        if (std::strncmp(buf, "Key up", 6) == 0) {
            int keyCode = _getKeyCode(buf);

            if (keyCode == 164){  // LEWY

                INPUT input;

                // Zwolnienie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_LMENU;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko zwolnienia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

            if (keyCode == 165){  // PRAWY

                INPUT input;

                // Zwolnienie klawisza Windows
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = VK_RMENU;  // Kod klawisza Windows
                input.ki.wScan = 0;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;

                // Symulowanie tylko zwolnienia klawisza Windows
                SendInput(1, &input, sizeof(INPUT));


                return;
            }

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

            if (keyCode == 162){
    
                std::cout << "CTRL" << std::endl;
                INPUT input;

                // Wciśnięcie klawisza Ctrl
                // inputs[0].type = INPUT_KEYBOARD;
                // inputs[0].ki.wVk = VK_CONTROL;  // Kod klawisza Ctrl
                // inputs[0].ki.wScan = 0;
                // inputs[0].ki.dwFlags = 0;
                // inputs[0].ki.time = 0;
                // inputs[0].ki.dwExtraInfo = 0;

                // Zwolnienie klawisza Ctrl
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

            std::cout << "Key Code UP: " << keyCode << std::endl;

            keybd_event(keyCode, 0, 0, 0);
        }

        /// ****           KEY DOWN             **** ///

        if (std::strncmp(buf, "Key down", 8) == 0) {    

            int keyCode = _getKeyCode(buf);                 // POBRANIE INFORMACJI O KODZIE KLAWISZA

            //      ***     LEWY CTRL    ***      //
            //      ***     PRAWY CTRL    ***      //
            //      ***     LEWY ALT    ***      //
            //      ***     PRAWY ALT    ***      //
            //      ***     WINDOWS    ***      //
            if (    keyCode == 162 
                 || keyCode == 163
          //       || keyCode == 164 
          //       || keyCode == 165
                // || keyCode == 91
             //    || keyCode == 161
             //    || keyCode == 160
                 )
            {

                // SYMULACJA CIĄGŁĘGO WCIŚNIĘCIA DANEGO PRZYCISKU

                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(keyCode, 0, 0, 0);
            
                return;
            }

            if (keyCode == 161){ // PRAWY SHIFT
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

            if (keyCode == 160){ // LEWY SHIFT
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

            if (keyCode == 164){ // LEWY ALT 
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

            if (keyCode == 165){ 
                std::cout << "FLAGA_1" << std::endl;
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

            if ( keyCode == 91 ){

                keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(keyCode, 0, 0, 0);
            
                return;
            }


            std::cout << "Key Code DOWN: " << keyCode << std::endl;
                    
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

