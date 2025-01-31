#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Socket creation failed." << endl;
        return 0;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(5000);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Binding failed." << endl;
        close(serverSocket);
        return 0;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed." << endl;
        close(serverSocket);
        return 0;
    }

    sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
    if (clientSocket < 0) {
        cerr << "Acceptance failed." << endl;
        close(serverSocket);
        return 0;
    }

    char buffer[1024] = {0};
    string s;
    while (true) {
        // First, receive a message from the client
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            cout << "Client disconnected or error in receiving." << endl;
            break;
        }
        cout << "Client: " << buffer << endl;

        // Prompt server user to send a message in response
        cout << "Send a message to the client or type 'quit' to close the connection: ";
        getline(cin, s);
        if (s == "quit") {
            break;
        }
        send(clientSocket, s.c_str(), s.length(), 0);
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}
