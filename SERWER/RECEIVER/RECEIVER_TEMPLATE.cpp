#pragma comment (lib, "ws2_32.lib")

using namespace std;

class RECEIVER {
    public:

    void clearCurrentLine() {
        std::cout << "\033[2K\r";
        std::cout.flush();
    }

    virtual void _receivedData(char * buf){
        /*
            -> ta metoda jest wywoływana po odebraniu wiadomości !
        */
        clearCurrentLine();
        cout << "<: " << buf << endl;
        cout << ">: ";
    }

    void _receiveData(int port_number) {
        /*
            -> metoda z nieskończoną pętlą, która nasłuchuje na danym porcie,
            -> gdy jakaś informacja zostanie odebrana, wywołuje się metoda _receivedData().
        */

        // Startup Winsock
        WSADATA data;
        WORD version = MAKEWORD(2,2);
        int wsOk = WSAStartup(version, &data);
        if (wsOk != 0){
            cout << "Can't start Winsock" << wsOk;
        }
        // Bind socket to ip address and port
        SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
        sockaddr_in serverHint;
        serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
        serverHint.sin_family = AF_INET;
        serverHint.sin_port = htons(port_number); // Convert from little to big endian

        if (bind (in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR){
            cout << "Can't bind socket! " << WSAGetLastError() << endl;
        }

        sockaddr_in client;
        int clientLength = sizeof(client);
        ZeroMemory(&client, clientLength);
    
        char buf[1024];

        // Enter a loop
        while(true){
            ZeroMemory(buf, 1024);

            // Wait for message
            int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
            if (bytesIn == SOCKET_ERROR){
                cout << "Error receiving from client " << WSAGetLastError() << endl;
                continue; 
            }

            // Display message and client info
            char clientIp[256];
            ZeroMemory(clientIp, 256);

            inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

            _receivedData(buf);
        }

        // Close socket
        closesocket(in);

        // Shutdown windock
        WSACleanup();
    }

};