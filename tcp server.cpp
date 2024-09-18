#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")  // Link with Ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    char buffer[1024];
    int clientLen, recvSize;

    // Step 1: Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    // Step 2: Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Step 3: Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Step 4: Bind
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 5: Listen for incoming connections
    listen(serverSocket, 3);
    printf("Waiting for incoming connections...\n");

    // Step 6: Accept incoming connection
    clientLen = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientLen);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Connection accepted.\n");

    // Step 7: Receive data from client
    recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvSize == SOCKET_ERROR) {
        printf("Recv failed with error code: %d\n", WSAGetLastError());
    } else {
        buffer[recvSize] = '\0';  // Null-terminate the received data
        printf("Client message: %s\n", buffer);
    }

    // Step 8: Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}