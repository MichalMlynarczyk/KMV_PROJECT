#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#elif _WIN32_WINNT < 0x0600
    #undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;


class SENDER_TEMP{
    
    public:

    // ZMIENNE
        WSADATA data;
        WORD version;
        sockaddr_in server;
        SOCKET out;
        unsigned int port_number;

    // METODY

    void _startupWinsock(){
        version = MAKEWORD(2,2);
        int wsOk = WSAStartup(version, &data);

        if (wsOk != 0){
            cout << "Can't start Winsock" << wsOk;
            return;
        }
    }

    void _createHintStructure(int port_number, string ip_adress){
        this -> port_number = port_number;
        // Crete a hint structure for the server
        server.sin_family = AF_INET;
        server.sin_port = htons(port_number);
        inet_pton(AF_INET, ip_adress.c_str(), &server.sin_addr);
        
        // Socket creation
        out = socket(AF_INET, SOCK_DGRAM, 0);
    }

    void _send(string message){
        // Write out to that socket
        int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(server));
    }

    void _shutdown(){
        // Close the socket
        closesocket(out);

        // Shutdown windock
        WSACleanup();
    }

};

