#include "RECEIVER_TEMPLATE.cpp"

class RECEIVER_EXA{
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

    RECEIVER_EXA(){

        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        clearScreen();

        // Pobierz  NR PORTU
        string PORT;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Podaj numer portu: ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        getline(cin, PORT);

        RECEIVER receiver;
        receiver._receiveData(stoi(PORT));
    }
};

int main() {
    RECEIVER_EXA recv;
}