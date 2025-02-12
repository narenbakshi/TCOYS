#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BAUDRATE B9600
#define PORT "/dev/rfcomm0"

int main() {
    int fd;
    struct termios serial;

    // Open the serial port
    fd = open(PORT, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        std::cerr << "Error opening serial port!" << std::endl;
        return 1;
    }

    // Configure the serial port
    tcgetattr(fd, &serial);
    cfsetospeed(&serial, BAUDRATE);
    cfsetispeed(&serial, BAUDRATE);
    serial.c_cflag &= ~PARENB; // No parity
    serial.c_cflag &= ~CSTOPB; // 1 stop bit
    serial.c_cflag &= ~CSIZE;
    serial.c_cflag |= CS8; // 8 data bits
    tcsetattr(fd, TCSANOW, &serial);

    std::cout << "Bluetooth communication started. Type 'exit' to quit." << std::endl;

    std::string input;
    char buffer[256];
    while (true) {
        // Send data to the phone
        std::cout << "Enter message: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        write(fd, input.c_str(), input.size());

        // Receive data from the phone
        int n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    close(fd);
    std::cout << "Bluetooth communication ended." << std::endl;
    return 0;
}
