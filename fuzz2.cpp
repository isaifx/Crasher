#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void sendBufferOverflowData(const char* target_ip, int target_port, int num_a) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(target_port);
    serverAddr.sin_addr.s_addr = inet_addr(target_ip); // Use inet_addr for IPv4 addresses

    try {
        // Connect to the target
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            throw std::runtime_error("Connection failed. Make sure the target is running and the IP and port are correct.");
        }

        std::cout << "Connected to the target." << std::endl;

        // Send 'A' characters for buffer overflow
        std::string buffer(num_a, 'A');
        const char* data = buffer.c_str();

        // Send the data
        send(clientSocket, data, num_a, 0);
        std::cout << "Sent " << num_a << " 'A' characters to the target for buffer overflow examination." << std::endl;

        // Close the connection
        closesocket(clientSocket);
        std::cout << "Connection closed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        closesocket(clientSocket);
    }

    // Cleanup Winsock
    WSACleanup();
}

int main() {
    // Get user input
    char target_ip[100];
    int target_port, num_a;

    std::cout << "Enter the target IP: ";
    std::cin >> target_ip;

    std::cout << "Enter the target port: ";
    std::cin >> target_port;

    std::cout << "Enter the number of 'A' characters to send for buffer overflow examination: ";
    std::cin >> num_a;

    // Call the function to send 'A' characters for buffer overflow
    sendBufferOverflowData(target_ip, target_port, num_a);

    return 0;
}