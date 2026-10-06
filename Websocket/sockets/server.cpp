#include "common.h"

int main(int argc, const char *argv[]) {
    // Validate command-line arguments
    checkargs(argc, argv);

    // Server and client socket file descriptors
    int obj_server, sock;
    
    // Structure to store server address information
    struct sockaddr_in address;
    
    // Socket option to reuse address
    int opted = 1;
    
    // Length of address structure
    socklen_t address_length = sizeof(address);
    
    // Buffer for receiving data
    char buffer[BUFFER_SIZE] = {0};
    
    // Message to send back to client
    const char *message = "Hi I am a message from the server!";

    // Create server socket
    // AF_INET: IPv4, SOCK_STREAM: TCP
    if ((obj_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        pserror("Opening of Socket Failed !");
    }

    // Allow reusing socket address
    if (setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted))) {
        pserror("Can't set the socket");
    }

    // Configure server address
    address.sin_family = AF_INET;             // IPv4
    address.sin_addr.s_addr = INADDR_ANY;     // Accept connections on any interface
    address.sin_port = htons(PORT);            // Port number

    // Bind socket to the specified network interface and port
    if (bind(obj_server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        pserror("Binding of socket failed !");
    }

    // Listen for incoming connections (max 3 queued)
    if (listen(obj_server, 3) < 0) {
        pserror("Can't listen from the server !");
    }

    // Accept incoming client connection
    if ((sock = accept(obj_server, (struct sockaddr *)&address, &address_length)) < 0) {
        pserror("Accept");
    }

    // Read message from client
    ssize_t reader = read(sock, buffer, BUFFER_SIZE);
    if (reader > 0) {
        std::cout << buffer << std::endl;
    }

    // Send response back to client
    send(sock, message, strlen(message), 0);
    std::cout << "SERVER: I just sent a message to the client!" << std::endl;

    // Close client and server sockets
    close(sock);
    close(obj_server);

    return 0;
}