#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

// Constant for the network port used in communication
#define PORT 8080

// Common utility function to validate command-line arguments
// Prevents unexpected arguments and provides diagnostic information
void checkargs(int argc, const char *argv[]) {
    if (argc > 1) {
        std::cerr << "You naughty little elf, this program takes no command-line arguments. :/" << std::endl;
        for (int i = 0; i < argc; ++i) {
            std::cout << "\targument " << i << ": " << argv[i] << std::endl;
        }
        exit(1);
    }
}

// Error handling utility function for server-side errors
// Prints error message and exits the program
void pserror(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Buffer size constant for network communication
const int BUFFER_SIZE = 1024;

#endif // COMMON_H
