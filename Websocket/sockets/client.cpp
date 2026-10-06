#include "common.h"

int main(int argc, const char *argv[]) {
    // Validate that no command-line arguments are provided
    checkargs(argc, argv);

    // Socket file descriptor
    int obj_socket, reader;
    
    // Structure to store server address information
    struct sockaddr_in serv_addr;
    
    // Message to be sent to the server
    const char *message = "CLIENT: I am sending a message!";
    
    // Buffer to store received data
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    // AF_INET: IPv4 protocol
    // SOCK_STREAM: TCP protocol (reliable, connection-oriented)
    if ((obj_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Configure server address structure
    serv_addr.sin_family = AF_INET;           // Address family (IPv4)
    serv_addr.sin_port = htons(PORT);         // Port number (network byte order)

    // Local loopback IP address (connects to same machine)
    const char *ip_address = "127.0.0.1";

    // Convert IP address from text to binary format
    // inet_pton: Presentation to Network conversion
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Establish connection to the server
    // Connects socket to the specified server address
    if (connect(obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Send message to the server
    // strlen(message): Send exact message length
    send(obj_socket, message, strlen(message), 0);
    std::cout << "CLIENT: Message has been sent!" << std::endl;

    // Read response from the server
    reader = read(obj_socket, buffer, BUFFER_SIZE);
    if (reader > 0) {
        std::cout << "CLIENT: printing the buffer..." << std::endl;
        std::cout << buffer << std::endl;
    }

    // Close the socket connection
    close(obj_socket);

    return 0;
}