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

#include "KMV/MOUSE_SUPPORT.cpp" 
#include "RECEIVER/RECEIVER_TEMPLATE.cpp"
#include "SENDER/SENDER_TEMPLATE.cpp"
#include "KMV/MOUSE_RAW_SENDER.cpp"

using namespace std;

HANDLE hConsole;
string position;

string CLIENT_PORT;
string RECEIVER_PORT;
string CLIENT_IP;

void _clearScreen() {
    /*
        DO CZYSZCZENIA KONSOLI
    */

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

void _downloadInformation(){
    /*
        POBIERA INFORMACJE O PORCIE KTÓRY NASŁUCHUJE
            O PORCIE KTÓRY WYSYŁA
            O IP NA KTÓRE WYSYŁAMY WIADOMOŚĆI
    */

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    _clearScreen();                                     // CZYSZCZENIE EKRANU

    // Pobierz  NR PORTU
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer portu na ktorym bedziemy nasluchiwac: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, RECEIVER_PORT);
    // Pobierz  IP
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer IP na ktory dane beda wysylane: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, CLIENT_IP);
    // Pobierz  NR PORTU
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer portu do wysylania danych: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, CLIENT_PORT);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int main(){

    _downloadInformation();         // POBIERANIE INFORMACJI O PORTACH I NUMERACH IP

    // ** ** ŁĄCZENIE DWÓCH KOMPUTERÓW ** ** //

    MOUSE_RAW_SENDER mouse_raw_sender_left;
    std::thread mouseRawSenderThreadLeft([&mouse_raw_sender_left]() {
    mouse_raw_sender_left._start(CLIENT_PORT, CLIENT_IP, "LEFT");});        // WYSYŁANIE INFORMACJI O RUCHACH MYSZKI DO KLIENTA

    MOUSE_RAW_SENDER mouse_raw_sender_right;
    std::thread mouseRawSenderThreadRight([&mouse_raw_sender_right]() {
    mouse_raw_sender_right._start(CLIENT_PORT, CLIENT_IP, "RIGHT");});        // WYSYŁANIE INFORMACJI O RUCHACH MYSZKI DO KLIENTA

    mouseRawSenderThreadLeft.join();                               // POCZEKAJ NA ZAKOŃCZENIE WĄTKU
    mouseRawSenderThreadRight.join();                               // POCZEKAJ NA ZAKOŃCZENIE WĄTKU

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}