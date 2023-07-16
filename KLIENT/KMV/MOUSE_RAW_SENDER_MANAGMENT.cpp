
/*
    ** -- KLASA MOUSE RAW SENDER MANAGMENT -- **

    -> DYNAMICZNIE TWORZY OBIEKTY             <-
        MOUSE_RAW_SENDER                      <-
*/

class MOUSE_RAW_SENDER_MANAGMENT{
    public:

    string IP_1 = "192.168.1.26";
    string IP_2 = "192.168.1.23";
    string PORT_1 = "5100";
    string PORT_2 = "5300";

    string PORT_11 = "6100";
    string PORT_21 = "6300";

    /////////////////////////////////////////////
    /////////////////////////////////////////////

    void _start(){
        _expect();
    }

    /////////////////////////////////////////////
    /////////////////////////////////////////////

    void _expect(){
        /*
            OCZEKUJUE NA ODPOWIEDNIĄ POZYCJĘ MYSZKI 
            I TWORZY OBIEKT MOUSE RAW SENDER
        */

        int MAX_SCREEN_WIDTH = GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1;         // POBRANIE SZEROKOŚCI EKRANU

        while (true){

            POINT currentMousePosition;
            GetCursorPos(&currentMousePosition);                                 // POBIERANIE AKTUALNEJ POZYCJI MYSZKI  

            if (currentMousePosition.x == 0){

                // ** MOUSE RAW SENDER ** //
                MOUSE_RAW_SENDER mouse_raw_sender;
                std::thread mouseRawSenderThread([&mouse_raw_sender]() {
                mouse_raw_sender._start("5100", "192.168.1.26", "LEFT");}); 

                // ** INPUT LISTENER ** // 
                // INPUT_LISTENER input_listener;
                // std::thread inputListenerThread([&input_listener]() {
                // input_listener._start(6100, "192.168.1.26", "LEFT");}); 


                mouseRawSenderThread.join();
                // inputListenerThread.join();
            }
            else if (currentMousePosition.x == MAX_SCREEN_WIDTH){

                // ** MOUSE RAW SENDER ** //
                MOUSE_RAW_SENDER mouse_raw_sender;
                std::thread mouseRawSenderThread([&mouse_raw_sender]() {
                mouse_raw_sender._start("5300", "192.168.1.23", "RIGHT");});

                // ** INPUT LISTENER ** //
                // INPUT_LISTENER input_listener;
                // std::thread inputListenerThread([&input_listener]() {
                // input_listener._start(6300, "192.168.1.23", "LEFT");}); 

                mouseRawSenderThread.join();
          //      inputListenerThread.join();
            }

        }
    }

};