#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#elif _WIN32_WINNT < 0x0600
    #undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include "../RECEIVER/RECEIVER_TEMPLATE.cpp"

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

    HANDLE hConsole;                                   // OBŁUGA KONSOLI

    void _start(){

        /*
            URUCHAMOMIENIE NASŁUCHIWANIA
        */

        // POIERANIE NUMERU PORTU                           //
        int PORT_NUMBER = _downloadPortNumber();

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

    int _downloadPortNumber(){
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        clearScreen();

        // Pobierz  NR PORTU
        string PORT;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Podaj numer portu na ktorym bedziemy nasluchiwac: ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        getline(cin, PORT);

        return stoi(PORT);
    }

    void clearScreen() {
        COORD coordScreen = { 0, 0 };
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition(hConsole, coordScreen);
    }

};

int main(){
    MOUSE_RAW_RECEIVER mouseRECEIVER;
    mouseRECEIVER._start();
}