
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

    void _receivedData(char *buf) override {

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

