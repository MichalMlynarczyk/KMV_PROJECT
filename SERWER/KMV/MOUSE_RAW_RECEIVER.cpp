#pragma comment (lib, "ws2_32.lib")

using namespace std;

/*
    ** -- KLASA MOUSE RAW RECEIVER -- **

    -> NASŁUCHIWANIE NA  DANYM PORCIE                                   <-
    -> PRZETWARZANIA I STEROWANIE MYSZKĄ NA KOMPUTERZE UŻYTKOWNIKA      <-
    -> W ZALEŻNOŚCI OD ARGUMENTU, ALBO NALICZAĆ MYSZKĘ Z LEWEJ, ALBO
                            PRAWEJ STRONY                               <-
    -> DZIEDZICZY PO KLASIE RECEIVER_TEMPLATE                           <-
*/


class MOUSE_RAW_RECEIVER : public RECEIVER{
    public:

    void _start(int PORT_NUMBER){

        /*
            URUCHAMOMIENIE NASŁUCHIWANIA
        */

        // ODBIERANIE DANYCH NA DANYM PORCIE                //
        // WYWOŁUJE ONA FUNKCE -- _RECEIVEDDATA(DATA) --    //
        // PRZY ODBIERANIU WIADOMOŚCI                       // 
        _receiveData(PORT_NUMBER);




    }

    void _receivedData(char * buf) override{
        /*
            NADPISANIE METODY KLASY BAZOWEJ DO ODBIERANIA WIADOMOŚĆI
        */

        int * VECTORS = _charToInt(buf);            // ZAMIANIA WIADOMOŚCI NA TABLICE INTÓW
        _controlMousePosition(VECTORS);             // PRZESUNIĘCIE MYSZKI O DANY WEKTOR
    }

    void _controlMousePosition(int * VECTORS){

        /*
            PRZESUWA MYSZKĘ Z OBECNEJ POZYCJI O DANY WEKTOR
        */

        POINT currentMousePosition;
        GetCursorPos(&currentMousePosition);         // POBRANIE I ZAPISANIE OBECNEJ POZYCJI MYSZKI

        POINT newMousePosition;
        newMousePosition.x = currentMousePosition.x + VECTORS[0];
        newMousePosition.y = currentMousePosition.y + VECTORS[1];       // WYLICZENIE NOWEJ POZYCJI MYSZKI

        SetCursorPos(newMousePosition.x, newMousePosition.y);           // PRZESUNIĘCIE MYSZKI
    }

    int * _charToInt(char * buf){

        /*
            ZAMIANIA ODEBRANEJ WIADOMOŚĆI NA TABLICE INTÓW
        */


        int *tab = new int[2];
        string sx,sy;
        bool end = false;

        for(int i = 0; i < strlen(buf); i++){
            if (end == false){
                if (buf[i] != ' '){
                    sx += buf[i];
                }
                else if (buf[i] == ' '){
                    end = true;
                }
            } else {
                if (buf[i] != ' '){
                    sy += buf[i];
                }
            }
        }

        tab[0] = stoi(sx);
        tab[1] = stoi(sy);

        return tab;
    }

};
