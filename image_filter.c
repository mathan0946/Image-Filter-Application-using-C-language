#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    char signature[2];
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
} BMPHeader;

typedef struct {
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

void readBMPHeader(FILE *file, BMPHeader *header) {
    fread(header, sizeof(BMPHeader), 1, file);
}

void readBMPInfoHeader(FILE *file, BMPInfoHeader *infoHeader) {
    fread(infoHeader, sizeof(BMPInfoHeader), 1, file);
}

void readPixelData(FILE *file, unsigned char **pixelData, int width, int height) {
    int row, col;
    *pixelData = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
    fseek(file, 54, SEEK_SET);

    for (row = 0; row < height; row++) {
        for (col = 0; col < width * 3; col += 3) {
            fread(&(*pixelData)[row * width * 3 + col], sizeof(unsigned char), 1, file); // Blue
            fread(&(*pixelData)[row * width * 3 + col + 1], sizeof(unsigned char), 1, file); // Green
            fread(&(*pixelData)[row * width * 3 + col + 2], sizeof(unsigned char), 1, file); // Red
        }
    }
}

void applyMeanFilter(unsigned char *inputData, unsigned char *outputData, int width, int height) {
    // Apply mean filter (3x3)
    int row, col, k, l;
    for (row = 1; row < height - 1; row++) {
        for (col = 3; col < width * 3 - 3; col += 3) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    sumB += inputData[(row + k) * width * 3 + col + (l * 3)];
                    sumG += inputData[(row + k) * width * 3 + col + (l * 3) + 1];
                    sumR += inputData[(row + k) * width * 3 + col + (l * 3) + 2];
                }
            }
            outputData[row * width * 3 + col] = sumB / 9;
            outputData[row * width * 3 + col + 1] = sumG / 9;
            outputData[row * width * 3 + col + 2] = sumR / 9;
        }
    }
}

unsigned char getMedian(unsigned char *data, int size) {
    // Sort the array to find the median
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                unsigned char temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }

    return data[size / 2];
}

void applyMedianFilter(unsigned char *inputData, unsigned char *outputData, int width, int height) {
    // Apply median filter (3x3)
    int row, col, k, l;
    unsigned char window[9];
    for (row = 1; row < height - 1; row++) {
        for (col = 3; col < width * 3 - 3; col += 3) {
            int index = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    window[index] = inputData[(row + k) * width * 3 + col + (l * 3)];
                    index++;
                }
            }
            outputData[row * width * 3 + col] = getMedian(window, 9);

            index = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    window[index] = inputData[(row + k) * width * 3 + col + (l * 3) + 1];
                    index++;
                }
            }
            outputData[row * width * 3 + col + 1] = getMedian(window, 9);

            index = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    window[index] = inputData[(row + k) * width * 3 + col + (l * 3) + 2];
                    index++;
                }
            }
            outputData[row * width * 3 + col + 2] = getMedian(window, 9);
        }
    }
}

void createNewBMP(char *outputFilename, unsigned char *pixelData, int width, int height, BMPHeader header, BMPInfoHeader infoHeader) {
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Unable to create file %s.\n", outputFilename);
        exit(1);
    }

    fwrite(&header, sizeof(BMPHeader), 1, outputFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    fseek(outputFile, header.dataOffset, SEEK_SET);
    fwrite(pixelData, sizeof(unsigned char), width * height * 3, outputFile);

    fclose(outputFile);
}

int main() {
    FILE *file = fopen("Your_path\\charlie.bmp", "rb");
    if (!file) {
        printf("Unable to open file.\n");
        return 1;
    }
    //Declaring Variable for Structure
    BMPHeader header;
    BMPInfoHeader infoHeader;

    readBMPHeader(file, &header);
    readBMPInfoHeader(file, &infoHeader);

    int width = infoHeader.width;
    int height = infoHeader.height;

    unsigned char *pixelData;
    readPixelData(file, &pixelData, width, height);
    fclose(file);

    unsigned char *meanFilteredData = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
    unsigned char *medianFilteredData = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
    unsigned char *meanANDmedianFilteredData = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));

    //Mean Filter
    applyMeanFilter(pixelData, meanFilteredData, width, height);
    //Median Filter
    applyMedianFilter(pixelData, medianFilteredData, width, height);

    createNewBMP("Your_path\\meanoutput.bmp", meanFilteredData, width, height, header, infoHeader);
    createNewBMP("Your_path\\medianoutput.bmp", medianFilteredData, width, height, header, infoHeader);
    //mean AND median
    applyMedianFilter(pixelData, medianFilteredData, width, height);
    applyMeanFilter(medianFilteredData, meanANDmedianFilteredData, width, height);

    createNewBMP("Your_path\\meanANDmedian.bmp", meanANDmedianFilteredData, width, height, header, infoHeader);
    printf("Output File Created...");
    free(pixelData);
    free(meanFilteredData);
    free(medianFilteredData);
    free(meanANDmedianFilteredData);

    return 0;
}
