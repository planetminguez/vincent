#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define KERNEL_SPACE_START 0xFFFF000000000000 // Typical kernel space start on x86_64
#define READ_SIZE 4096                       // Number of bytes to read at a time

void read_kernel_memory(uint64_t start_address, uint64_t end_address) {
    int mem_fd = open("/dev/mem", O_RDONLY);
    if (mem_fd < 0) {
        perror("Failed to open /dev/mem. Are you running as root?");
        return;
    }

    uint64_t address;
    unsigned char buffer[READ_SIZE];

    for (address = start_address; address < end_address; address += READ_SIZE) {
        if (lseek(mem_fd, address, SEEK_SET) == -1) {
            perror("lseek failed");
            break;
        }

        ssize_t bytes_read = read(mem_fd, buffer, READ_SIZE);
        if (bytes_read < 0) {
            perror("Read failed");
            break;
        }

        printf("Address: 0x%016lx | Data: ", address);
        for (int i = 0; i < bytes_read; i++) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
    }

    close(mem_fd);
}

int main() {
    printf("Kernel memory reader. Note: Run this program as root.\n");
    printf("Reading kernel memory space...\n");

    // Define the kernel memory range to read (adjust as needed)
    uint64_t start_address = KERNEL_SPACE_START;
    uint64_t end_address = KERNEL_SPACE_START + (16 * 1024); // Limit to 16 KB for safety

    read_kernel_memory(start_address, end_address);

    printf("Kernel memory read complete.\n");
    return 0;
}
