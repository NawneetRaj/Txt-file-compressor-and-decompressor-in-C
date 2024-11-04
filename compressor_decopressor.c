#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Compress(unsigned char *output, const char *input) {
    unsigned long long buffer = 0; 
    char bitsInBuffer = 0;
    while (*input != '\0') {
        buffer |= (unsigned long long)(*input++ & 0x7F) << bitsInBuffer;
        bitsInBuffer += 7; 

        if (bitsInBuffer >= 56) { 
            while (bitsInBuffer >= 8) {
                *output++ = buffer & 0xFF; 
                buffer >>= 8; 
                bitsInBuffer -= 8; 
            }
            buffer = 0; 
            bitsInBuffer = 0; 
        }
    }
    
    while (bitsInBuffer > 0) {
        *output++ = buffer & 0xFF; 
        buffer >>= 8; 
        bitsInBuffer -= 8; 
    }
}

void Uncompress(char *output, const unsigned char *input, unsigned compressedLength) {
    unsigned long long buffer = 0; 
    char bitsInBuffer = 0;

    while (compressedLength > 0) {
        while (compressedLength > 0 && bitsInBuffer < 56) {
            buffer |= (unsigned long long)(*input++) << bitsInBuffer;
            bitsInBuffer += 8; 
            compressedLength--; 
        }
        
        while (bitsInBuffer >= 7) {
            *output++ = buffer & 0x7F; 
            buffer >>= 7; 
            bitsInBuffer -= 7; 
        }
    }
    *output = '\0'; 
}

int main() {
    char inputMessage[100];  
    unsigned char compressedData[100]; 
    char decompressedMessage[100];

    FILE *inputFile = fopen("file.txt", "r"); 
    if (inputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fgets(inputMessage, sizeof(inputMessage), inputFile);
    fclose(inputFile);
    
    printf("Original Message: %s\n", inputMessage);
    
    Compress(compressedData, inputMessage);
    printf("Compressed Data: ");
    for (int i = 0; i < sizeof(compressedData); i++) {
        printf("%02X ", compressedData[i]); 
    }
    printf("\n");

    Uncompress(decompressedMessage, compressedData, sizeof(compressedData));
    printf("Decompressed Message: %s\n", decompressedMessage);

    return 0;
}
