#include <sstream> 

/*
    ** -- KLASA MOUSE RAW SENDER MANAGMENT -- **

    -> DYNAMICZNIE TWORZY OBIEKTY             <-
        MOUSE_RAW_SENDER                      <-
*/

class MOUSE_RAW_SENDER_MANAGMENT{
    public:

    /////////////////////////////////////////////
    /////////////////////////////////////////////

    void _start(string * COMPUTERS_IP_NUMBER, string * COMPUTER_DIRECTION, int COMPUTERS_NUMBER){
        _expect(COMPUTERS_IP_NUMBER, COMPUTER_DIRECTION, COMPUTERS_NUMBER);
    }

    /////////////////////////////////////////////
    /////////////////////////////////////////////

    void _expect(string * COMPUTERS_IP_NUMBER, string * COMPUTER_DIRECTION, int COMPUTERS_NUMBER){
        /*
            OCZEKUJUE NA ODPOWIEDNIĄ POZYCJĘ MYSZKI 
            I TWORZY OBIEKT MOUSE RAW SENDER
        */

        int MAX_SCREEN_WIDTH = GetSystemMetrics(SM_CXVIRTUALSCREEN) - 1;         // POBRANIE SZEROKOŚCI EKRANU

        while (true){

            POINT currentMousePosition;
            GetCursorPos(&currentMousePosition);                                 // POBIERANIE AKTUALNEJ POZYCJI MYSZKI 

            for (int i = 0; i < COMPUTERS_NUMBER; i++){

                if (currentMousePosition.x == 0 && COMPUTER_DIRECTION[i] == "LEFT"){

                    // ** WYSYŁANIE WIADOMOŚĆI O ZMINA POZYCJi KURSORA ** //
                    INPUT_LISTENER input;
                    input._startupWinsock();
                    input._createHintStructure(5100 + 1000, COMPUTERS_IP_NUMBER[i]);

                    // ** POBRANIE WYSOKOŚĆI MONITORA ** //
                    int monitorHeight = GetSystemMetrics(SM_CYSCREEN);
                    float monitor_proportion = static_cast<float>(currentMousePosition.y) /  static_cast<float>(monitorHeight);
                    std::ostringstream monitor_proportion_ostring;
                    monitor_proportion_ostring << monitor_proportion;
                    std::string monitor_proportion_string = monitor_proportion_ostring.str();

                    // ** WIADOMOŚĆ WYSYŁANA DO KLIENTA ** //
                    std::string msg = "MP: " + monitor_proportion_string;

                    // ** WYSYŁANIE DANYCH O POZYCJI KORSORA ** //
                    input._send(msg);
                    input._stop();

                    // ** MOUSE RAW SENDER ** //
                    MOUSE_RAW_SENDER mouse_raw_sender;
                    std::thread mouseRawSenderThread([&]() {
                    mouse_raw_sender._start("5100", COMPUTERS_IP_NUMBER[i], "LEFT");}); 

                    mouseRawSenderThread.join();
                }
                else if (currentMousePosition.x == MAX_SCREEN_WIDTH && COMPUTER_DIRECTION[i] == "RIGHT"){

                    // ** WYSYŁANIE WIADOMOŚĆI O ZMINA POZYCJi KURSORA ** //
                    INPUT_LISTENER input;
                    input._startupWinsock();
                    input._createHintStructure(5300 + 1000, COMPUTERS_IP_NUMBER[i]);

                    // ** POBRANIE WYSOKOŚĆI MONITORA ** //
                    int monitorHeight = GetSystemMetrics(SM_CYSCREEN);
                    float monitor_proportion = static_cast<float>(currentMousePosition.y) /  static_cast<float>(monitorHeight);
                    std::ostringstream monitor_proportion_ostring;
                    monitor_proportion_ostring << monitor_proportion;
                    std::string monitor_proportion_string = monitor_proportion_ostring.str();

                    // ** WIADOMOŚĆ WYSYŁANA DO KLIENTA ** //
                    std::string msg = "MP: " + monitor_proportion_string;

                    // ** WYSYŁANIE DANYCH O POZYCJI KORSORA ** //
                    input._send(msg);
                    input._stop();

                    // ** MOUSE RAW SENDER ** //
                    MOUSE_RAW_SENDER mouse_raw_sender;
                    std::thread mouseRawSenderThread([&]() {
                    mouse_raw_sender._start("5300", COMPUTERS_IP_NUMBER[i], "RIGHT");});

                    mouseRawSenderThread.join();
                }
            } 

            // if (currentMousePosition.x == 0){

            //     // ** MOUSE RAW SENDER ** //
            //     MOUSE_RAW_SENDER mouse_raw_sender;
            //     std::thread mouseRawSenderThread([&mouse_raw_sender]() {
            //     mouse_raw_sender._start("5100", "192.168.1.23", "LEFT");}); 

            //     mouseRawSenderThread.join();
            // }
            // else if (currentMousePosition.x == MAX_SCREEN_WIDTH){

            //     // ** MOUSE RAW SENDER ** //
            //     MOUSE_RAW_SENDER mouse_raw_sender;
            //     std::thread mouseRawSenderThread([&mouse_raw_sender]() {
            //     mouse_raw_sender._start("5300", "192.168.1.23", "RIGHT");});

            //     mouseRawSenderThread.join();
            // }

        }
    }

};