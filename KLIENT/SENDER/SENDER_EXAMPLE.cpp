#include "SENDER_TEMPLATE.cpp"

class SENDER_EXA{
    public:

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

    SENDER_EXA(){

        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        clearScreen();

        // Pobierz  IP
        string IP;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Podaj numer IP: ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        getline(cin, IP);
        // Pobierz  NR PORTU
        string PORT;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Podaj numer portu: ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        getline(cin, PORT);

        SENDER_TEMP send;
        send._startupWinsock();
        send._createHintStructure(stoi(PORT), IP);
        send._send("Hello, world!");
        send._shutdown();
    }
};

int main() {
    SENDER_EXA send;
}