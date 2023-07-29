#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#elif _WIN32_WINNT < 0x0600
    #undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif

#include <cctype>
#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <sstream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <cstring>

#include "RECEIVER/RECEIVER_TEMPLATE.cpp"
// #include "SENDER/SENDER_TEMPLATE.cpp"
#include "KMV/MOUSE_RAW_RECEIVER.cpp"
#include "COMUNICATION/INPUT_RECEIVER.cpp"
#include <thread>

using namespace std;

HANDLE      hConsole;
string      position;

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

int _downloadPortNumberInputListener(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Pobierz  NR PORTU
    string PORT;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer portu na ktorym bedziemy nasluchiwac INPUT LISTENER: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, PORT);

    return stoi(PORT);
}

int _downloadPortNumberMouseRaw(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    clearScreen();

    // Pobierz  NR PORTU
    string PORT;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer portu na ktorym bedziemy nasluchiwac MOUSE RAW: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, PORT);

    return stoi(PORT);
}

int main(int argc, char* argv[]) {

    //int MOUSE_RAW_PORT_NUMBER =         _downloadPortNumberMouseRaw();
    //int INPUT_LISTENER_PORT_NUMBER =    _downloadPortNumberInputListener();

    int MOUSE_RAW_PORT_NUMBER = std::atoi(argv[1]);
    int INPUT_LISTENER_PORT_NUMBER = std::atoi(argv[2]);

    // std::cout << "MOUSE_RAW_PORT_NUMBER: " << MOUSE_RAW_PORT_NUMBER << std::endl;
    // std::cout << "INPUT_LISTENER_PORT_NUMBER: " << INPUT_LISTENER_PORT_NUMBER << std::endl;

    // ** ODBIERANIE I PRZETWARZANIE MOUSE RAW ** //
    MOUSE_RAW_RECEIVER mouseRECEIVER;
    std::thread thread_1([&mouseRECEIVER, MOUSE_RAW_PORT_NUMBER]() {
        mouseRECEIVER._start(MOUSE_RAW_PORT_NUMBER);});  
    
    thread_1.detach();

    // ** ODBIERANIE I PRZETWARZANIE INPUT LISTENER ** //
    INPUT_RECEIVER inputRECEIVER;
    std::thread thread_2([&inputRECEIVER, INPUT_LISTENER_PORT_NUMBER]() {
        inputRECEIVER._start(INPUT_LISTENER_PORT_NUMBER);});  
    
    thread_2.join();

}