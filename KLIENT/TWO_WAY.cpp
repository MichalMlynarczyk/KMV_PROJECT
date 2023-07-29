#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#elif _WIN32_WINNT < 0x0600
    #undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <Windows.h>
#include <thread>

#include "SENDER/SENDER_TEMPLATE.cpp"
#include "KMV/INPUT_LISTENER.cpp"
#include "KMV/MOUSE_SUPPORT.cpp" 
#include "KMV/KEYBOARD.cpp"
#include "KMV/MOUSE_RAW_SENDER.cpp"
#include "KMV/MOUSE_RAW_SENDER_MANAGMENT.cpp"

using namespace std;

HANDLE hConsole;
string position;

string CLIENT_PORT;
string RECEIVER_PORT;
string CLIENT_IP;

//string COMPUTERS_NUMBER;                                // ILE KOMPUTERÓW MA BYĆ DODANE
//string * COMPUTERS_IP_NUMBER;                           // TABLICA Z NUMERAMI IP KOMPÓTERÓW
//string * COMPUTER_DIRECTION;                            // KIERUNEK DOŁĄCZENIA KOMPUTERA

// void _clearScreen() {
//     /*
//         DO CZYSZCZENIA KONSOLI
//     */

//     COORD coordScreen = { 0, 0 };
//     DWORD cCharsWritten;
//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     DWORD dwConSize;

//     GetConsoleScreenBufferInfo(hConsole, &csbi);
//     dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
//     FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
//     GetConsoleScreenBufferInfo(hConsole, &csbi);
//     FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
//     SetConsoleCursorPosition(hConsole, coordScreen);
// }

// void _downloadInformation(){
//     /*
//         POBIERA INFORMACJE O PORCIE KTÓRY NASŁUCHUJE
//             O PORCIE KTÓRY WYSYŁA
//             O IP NA KTÓRE WYSYŁAMY WIADOMOŚĆI
//     */

//     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     _clearScreen();                                     // CZYSZCZENIE EKRANU

//     // Pobierz  COMPUTERS NUMBER
//     SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//     cout << "Podaj ile komputerow chcesz dodac (MAX 2): ";
//     SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
//     getline(cin, COMPUTERS_NUMBER);

//     // TWORZENIE TABLICY STRINGÓW
//     COMPUTERS_IP_NUMBER = new string [stoi(COMPUTERS_NUMBER)];
//     COMPUTER_DIRECTION = new string [stoi(COMPUTERS_NUMBER)];

//     for (int i = 0; i < stoi(COMPUTERS_NUMBER); i++){

//         // Pobierz  COMPUTER IP
//         SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//         cout << "Podaj IP komputera: ";
//         SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
//         getline(cin, COMPUTERS_IP_NUMBER[i]);

//         // POBIERZ DIRECTION
//         SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//         cout << "Z jakiej strony dodac komputer (LEFT/RIGHT): ";
//         SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
//         getline(cin, COMPUTER_DIRECTION[i]);
//     }
// }

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){

    if (argc == 3){

        string COMPUTERS_IP_NUMBER[1];
        COMPUTERS_IP_NUMBER[0] = argv[1];

        string COMPUTER_DIRECTION[1];
        COMPUTER_DIRECTION[0] = argv[2];

        string COMPUTERS_NUMBER = "1";

        MOUSE_RAW_SENDER_MANAGMENT mouse_raw_sender_managment;
        mouse_raw_sender_managment._start(COMPUTERS_IP_NUMBER, COMPUTER_DIRECTION, stoi(COMPUTERS_NUMBER));

    }
    else if (argc == 5){

        string COMPUTERS_IP_NUMBER[2];
        COMPUTERS_IP_NUMBER[0] = argv[1];
        COMPUTERS_IP_NUMBER[1] = argv[3];

        string COMPUTER_DIRECTION[2];
        COMPUTER_DIRECTION[0] = argv[2];
        COMPUTER_DIRECTION[1] = argv[4];

        string COMPUTERS_NUMBER = "2";

        MOUSE_RAW_SENDER_MANAGMENT mouse_raw_sender_managment;
        mouse_raw_sender_managment._start(COMPUTERS_IP_NUMBER, COMPUTER_DIRECTION, stoi(COMPUTERS_NUMBER));
    }
     else {
        return 1;
    }
}

