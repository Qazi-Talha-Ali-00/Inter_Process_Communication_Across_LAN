#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Socket creation failed." << endl;
        return 0;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);

    if (inet_pton(AF_INET, "172.31.94.4", &serverAddress.sin_addr) <= 0) {
        cerr << "Invalid address." << endl;
        close(clientSocket);
        return 0;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Connection failed." << endl;
        close(clientSocket);
        return 0;
    }

    string s;
    char buffer[1024] = {0};
    while (true) {
        // First, send a message to the server
        cout << "Send a message to the server or type 'quit' to close the connection: ";
        getline(cin, s);
        if (s == "quit") {
            break;
        }
        send(clientSocket, s.c_str(), s.length(), 0);

        // Then, receive a message from the server
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            cout << "Server disconnected or error in receiving." << endl;
            break;
        }
        cout << "Server: " << buffer << endl;
    }

    close(clientSocket);
    return 0;
}
