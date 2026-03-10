#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
    FILE *streamIn = fopen("/home/mial/Downloads/snail.bmp", "rb");

    if (streamIn == NULL) {
        perror("Error opening file"); // Tells you WHY it failed (e.g., File not found)
        return 1;
    }

    printf("File stream opened successfully at memory address: %p\n", (void*)streamIn);
    printf("File : %d\n", &streamIn);

    // Always close what you open to avoid memory leaks/resource exhaustion
    fclose(streamIn);
    return 0;
}