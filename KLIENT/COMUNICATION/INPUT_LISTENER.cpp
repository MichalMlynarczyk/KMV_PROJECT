
/*
    ** -- KLASA INPUT LISTENER -- **

    -> ZBIERANIE DANYCH Z KLAWIATURY     <-
    -> ZBIERANIE DANYCH Z MYSZKI :       <-
    ->      PRAWY PRZYCISK MYSZKI        <-
    ->      LEWY PRZYCISK MYSZKI         <-
    ->      SCROLL MYSZKI                <-
    -> JEDNAK BĘDZIEMY KORZYSTAĆ Z UDP   <-

*/

class INPUT_LISTENER : public SENDER_TEMP{

    public:

    string DIRECTION;                           // Z JAKIEJ STRONY WYSYŁAĆ DANE
    bool stopFlag = false;                      // DO ZAKOŃCZENIA DZIAŁANIE OBIEKTU

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void _start(int PORT_NUMBER, string IP_NUMBER, string DIRECTION){
        
        this -> DIRECTION = DIRECTION;

        /*
            URUCHAMIANIE WYSYŁANIA DANYCH
        */

        _startupWinsock();
        _createHintStructure(PORT_NUMBER, IP_NUMBER);       // TWORZENIE POŁĄCZENIA TCP
        _sendData();
        _shutdown();

    }

    void _stop(){
        stopFlag = true;                        // ZATRZYMANIE
    }

    ~INPUT_LISTENER(){
        std::cout << "KONIEC INPUT LISTENER" << std::endl; 
    }

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void _sendData(){

        /*
            WYSYŁA DANE W ZALEŻNOŚCI CZY MYSZKA PRZEKROCZYŁA 
                LEWĄ STRONĘ EKRANU CZY PRAWĄ
        */

        _checkInput();

    }

    void _checkInput(){

        int LEFT_BUTTON_PRESSED = 0;        // STAN LEWEGO PRZYCISKU
        int RIGHT_BUTTON_PRESSED = 0;       // STAN PRAWEGO PRZYCISKU

        while (true && !stopFlag){

            // ** WYSYŁANIE DANYCH Z KLAWIATURY ** //
            _isKeyPressed();

            // ** WYKRYWANIE LEWEGO PRZYCISKU ** //
            if (_isLeftMouseButtonPressed() && LEFT_BUTTON_PRESSED == 0){
                _send("MOUSE_INPUT_LEFT_ON");
                std::cout << "MOUSE_INPUT_LEFT_ON" << std::endl;
                LEFT_BUTTON_PRESSED = 1;
            }
            else if (_isLeftMouseButtonPressed() == false && LEFT_BUTTON_PRESSED == 1){
                _send("MOUSE_INPUT_LEFT_OFF");
                std::cout << "MOUSE_INPUT_LEFT_OFF" << std::endl;
                LEFT_BUTTON_PRESSED = 0;
            }

            // ** WYKRYWANIE PRAWEGO PRZYCISKU ** //

            if (_isRighttMouseButtonPressed() && RIGHT_BUTTON_PRESSED == 0){
                _send("MOUSE_INPUT_RIGHT_ON");
                RIGHT_BUTTON_PRESSED = 1;
            }
            else if (_isRighttMouseButtonPressed() == false && RIGHT_BUTTON_PRESSED == 1){
                _send("MOUSE_INPUT_RIGHT_OFF");
                RIGHT_BUTTON_PRESSED = 0;
            }

            if (DIRECTION == "LEFT"){

                POINT cursorPosition;
                GetCursorPos(&cursorPosition);      // Pobranie aktualnej pozycji myszki

                if (cursorPosition.x != 0)
                   _stop();

            } else if (DIRECTION == "RIGHT"){
                POINT cursorPosition;
                GetCursorPos(&cursorPosition);      // Pobranie aktualnej pozycji myszki

                int MAX_SCREEN_WIDTH = GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1;

                if (cursorPosition.x != MAX_SCREEN_WIDTH)
                    _stop();
            } 

        }
    }

    bool _isLeftMouseButtonPressed(){

        short state = GetAsyncKeyState(VK_LBUTTON);         // POBIERZ STAN LEWEGO PRZYCISKU MYSZY

        bool isPressed = (state & 0x8000) != 0;             // OKREŚLENIE STANU PRZYCISKU

        return isPressed;

    }

    bool _isRighttMouseButtonPressed(){

        short state = GetAsyncKeyState(VK_RBUTTON);         // POBIERZ STAN PRAWEGO PRZYCISKU MYSZY

        bool isPressed = (state & 0x8000) != 0;             // OKREŚLENIE STANU PRZYCISKU

        return isPressed;
        
    }

    void _isKeyPressed(){

        // ** ZNAKI SPECJALNE ** //

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)           // ESCAPE
            _send("KEY ESCAPE");

        if (GetKeyState(VK_SPACE) & 0x8000)                 // SPACJA
            _send("KEY SPACE");

        // ** LITERY ** //

        if (GetAsyncKeyState('Q') & 0x8000)                 // Q
            _send("KEY Q");

        if (GetAsyncKeyState('W') & 0x8000)                 // W
            _send("KEY W");

        if (GetAsyncKeyState('E') & 0x8000)                 // E
            _send("KEY E");

        if (GetAsyncKeyState('R') & 0x8000)                 // R
            _send("KEY R");

        if (GetAsyncKeyState('T') & 0x8000)                 // T
            _send("KEY T");

        if (GetAsyncKeyState('Y') & 0x8000)                 // Y
            _send("KEY Y");

        if (GetAsyncKeyState('U') & 0x8000)                 // U
            _send("KEY U");

        if (GetAsyncKeyState('I') & 0x8000)                 // I
            _send("KEY I");

        if (GetAsyncKeyState('O') & 0x8000)                 // O
            _send("KEY O");

        if (GetAsyncKeyState('P') & 0x8000)                 // P
            _send("KEY P");

        if (GetAsyncKeyState('A') & 0x8000)                 // A
            _send("KEY A");

        if (GetAsyncKeyState('S') & 0x8000)                 // S
            _send("KEY S");

        if (GetAsyncKeyState('D') & 0x8000)                 // D
            _send("KEY D");

        if (GetAsyncKeyState('F') & 0x8000)                 // F
            _send("KEY F");

        if (GetAsyncKeyState('G') & 0x8000)                 // G
            _send("KEY G");

        if (GetAsyncKeyState('H') & 0x8000)                 // H
            _send("KEY H");

        if (GetAsyncKeyState('J') & 0x8000)                 // J
            _send("KEY J");

        if (GetAsyncKeyState('K') & 0x8000)                 // K
            _send("KEY K");

        if (GetAsyncKeyState('L') & 0x8000)                 // L
            _send("KEY L");

        if (GetAsyncKeyState('Z') & 0x8000)                 // Z
            _send("KEY Z");

        if (GetAsyncKeyState('X') & 0x8000)                 // X
            _send("KEY X");

        if (GetAsyncKeyState('C') & 0x8000)                 // C
            _send("KEY C");

        if (GetAsyncKeyState('V') & 0x8000)                 // V
            _send("KEY V");

        if (GetAsyncKeyState('B') & 0x8000)                 // B
            _send("KEY B");

    }

};

// int main(){

//     INPUT_LISTENER input_listener;
//     input_listener._start(6100, "192.168.1.23", "RIGHT");

// }