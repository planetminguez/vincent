#include <stdio.h>
#include <stdlib.h>

void dumpMemory(void *ptr, size_t size) {
    unsigned char *bytePtr = (unsigned char *)ptr;
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            printf("\n%p: ", bytePtr + i);  // Print memory address at the start of every new line
        }
        printf("%02x ", bytePtr[i]);
    }
    printf("\n");
}

int main() {
    size_t size = 64;  // Size of the memory block to dump
    void *memoryBlock = malloc(size);

    if (memoryBlock == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize the memory with some values for demonstration
    for (size_t i = 0; i < size; i++) {
        ((unsigned char*)memoryBlock)[i] = i;
    }

    printf("Memory Dump (bottom):\n");
    dumpMemory(memoryBlock, size);

    free(memoryBlock);  // Don't forget to free allocated memory
    return 0;
}
