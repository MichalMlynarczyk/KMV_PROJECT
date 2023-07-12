#include "RECEIVER/RECEIVER_TEMPLATE.cpp"
#include "SENDER/SENDER_TEMPLATE.cpp"
#include <thread>

using namespace std;

HANDLE hConsole;
string position;

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

int main(){

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    clearScreen();

    // Pobierz  NR PORTU
    string PORT;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << "Podaj numer portu na ktorym bedziemy nasluchiwac: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    getline(cin, PORT);

    RECEIVER receiver;
    thread receiverThread(&RECEIVER::_receiveData, &receiver, stoi(PORT));

    // Wykonuj dalej kod 
    receiverThread.detach();

    

}