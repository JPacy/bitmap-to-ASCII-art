#include <stdio.h>
#include <stdlib.h>

// pragma to get rid of memory padding
#pragma pack(push, 1)
// struct to store all file header data
typedef struct tagBitmapFileHeader {
    u_int16_t fileType;
    u_int32_t fileSize;
    u_int16_t fileReserved1;
    u_int16_t fileReserved2;
    u_int32_t bitmapOffset;
} bitmapFileHeader;

#pragma pack(pop)


#pragma pack(push, 1)
// struct to store all infoheader data
typedef struct tagBitmapInfoHeader {
    u_int32_t headerSize;
    int32_t bitmapWidth;
    int32_t bitmapHeight;
    u_int16_t bitmapPlanes;
    u_int16_t bitmapBitsPerPixel;
    u_int32_t bitmapCompressType;
    u_int32_t bitmapImageSize;
    int32_t bitmapPixelPerMeterX;
    int32_t bitmapPixelPerMeterY;
    u_int32_t bitmapColorsUsed;
    u_int32_t bitmapColorsImportant;
} bitmapInfoHeader;

#pragma pack(pop)

// struct to store rgb values from bitmap
typedef struct tagRGB {
    unsigned char redValue;
    unsigned char greenValue;
    unsigned char blueValue;
} rgb;

// struct to store imagedata
typedef struct tagImage {
    int height;
    int width;
    rgb **rgbValue;
} image;

// global variables for bmp header data
bitmapFileHeader fileHeader;
bitmapInfoHeader infoHeader;

// global variables lena
int imageArrayGreyscale[512][512];
char imageArray[512][512];

// array with all the symbols used for greyscale to ascii conversion
char symbolArray[65] = {'\"', ',', ':', ';', 'I', 'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?', ']', '[', '}', '{', '1', ')', '(', '|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z', 'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o', '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'};
int row, column, greyscale;
const int sizeLena = 512;

// enum for ansi colors
enum ansiColor{black, red, green, yellow, blue, purple, cyan, white} color;

// This function opens a .txt file and initializes the array values.
void readFile(char *fileName)
{
    // open file
    FILE *fptr;
    // print error message if file not found
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("Error! Could not open file.\n");
        exit(1);
    }
    // read greyscale values from .txt file in array
    while (fscanf(fptr, "%d %d %d", &column, &row, &greyscale) != EOF) {
        imageArrayGreyscale[column - 1][row - 1] = greyscale;
    }
    fclose(fptr);
}

// This function turns greyscale values into ascii symbols. 
void makeCharArray() 
{
    // loop through array 
    for (int x = 0; x < sizeLena; x++) {
        for (int y = 0; y < sizeLena; y++) {
                // assign ascii symbol to imageArray
                imageArray[x][y] = symbolArray[imageArrayGreyscale[x][y] / 4];
        }
    }
}

// add colour based on rgb
void addColour(int red, int green, int blue) 
{
    printf("\033[38;2;%d;%d;%dm", red, green, blue);
}

void addColourBitmap(rgb rgbValue) {
    printf("\033[38;2;%d;%d;%dm", rgbValue.blueValue, rgbValue.greenValue, rgbValue.redValue);
}

// add color, only ansi colors
void addColourAnsi(int color) 
{
    printf("\033[0;3%dm", color);
}

// reset color
void colourReset() 
{
    printf("\033[0m");
}

// This function prints the average greyscale value in a given range.
void averageGreyscale(int yStart, int yEnd, int xStart, int xEnd)
{
    int sumGreyscale = 0;
    float averageGreyscale;
    int numOfValues = 0;
    // loop through array
    for (int i = yStart; i <= yEnd; i++) {
        for (int j = xStart; j <= xEnd; j++) {
            // add value from imageArrayGreyscale to sumGreyscale
            sumGreyscale += imageArrayGreyscale[i][j];
            numOfValues++;
        }
    }
    
    averageGreyscale = sumGreyscale / numOfValues;
    printf("The average greyscale value within given range: %f\n", averageGreyscale);
}

// This function prints the image in ascii symbols.
void printAscii(int yStart, int yEnd, int xStart, int xEnd) 
{
    for (int i = yStart; i <= yEnd; i++) {
        for (int j = xStart; j <= xEnd; j++) {
            printf("%c", imageArray[i][j]);
        }
        printf("\n");
    }
}

// prints the image in ascii symbols mirrored.
void printAsciiMirrored(int yStart, int yEnd, int xStart, int xEnd)
{
    for (int i = yStart; i <= yEnd; i++) {
        for (int j = xEnd; j >= xStart; j--) {
            printf("%c", imageArray[i][j]);
        }
        printf("\n");
    }
}

// print image in color 
void printAsciiColour(int yStart, int yEnd, int xStart, int xEnd, int redValue, int greenValue, int blueValue) 
{
    for (int i = yStart; i <= yEnd; i++) {
        for (int j = xStart; j <= xEnd; j++) {
            addColour(redValue, greenValue, blueValue);
            printf("%c", imageArray[i][j]);
        }
        printf("\n");
    }
    colourReset();
}

// saves ascii image in txt file.
void saveImageAscii(char fileName[31], int yStart, int yEnd, int xStart, int xEnd) 
{
    FILE *fptr;
    fptr = fopen(fileName, "w+");

    if (fptr == NULL) {
        printf("Unable to create file.\n");
        exit(2);
    }
    
    for (int i = yStart; i < yEnd; i ++) {
        for (int j = xStart; j < xEnd; j++) {
            fprintf(fptr, "%c", imageArray[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

// saves ascii image (mirrored version) in txt file.
void saveImageAsciiMirrored(char fileName[31], int xStart, int xEnd, int yStart, int yEnd) 
{
    FILE *fptr;
    fptr = fopen(fileName, "w+");

    if (fptr == NULL) {
        printf("Unable to create file.\n");
        exit(2);
    }
    
    for (int i = xStart; i < xEnd; i ++) {
        for (int j = yEnd; j >= yStart; j--) {
            fprintf(fptr, "%c", imageArray[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

// convert rgb to greyscale
unsigned char rgbToGreyscale(rgb rgbValue) {
    return ((0.299 * rgbValue.redValue) + (0.587 * rgbValue.greenValue) + (0.114 * rgbValue.blueValue));
}

void bmpToAscii(image picture, char saveFile[31]) 
{
FILE *fptr;
// create a savefile
fptr = fopen(saveFile, "w+");

unsigned char greyscale;
int i, j;
// print image to savefile
for (i = 0; i < picture.height; i++) {
    for (j = 0; j < picture.width; j++) {
        greyscale = rgbToGreyscale(picture.rgbValue[i][j]);
        printf("%c", symbolArray[greyscale / 4]);
        fprintf(fptr , "%c", symbolArray[greyscale / 4]);
    }
    printf("\n");
    fprintf(fptr,"\n");
    }
    fclose(fptr);   
}

// function for reading bmp image data
image readImage(FILE *fptr, int height, int width)  
{
    image picture;
    int pixelRowSize, numOfPixels;
    // allocate memory to store rgb data
    picture.rgbValue = (rgb**) malloc(height * sizeof(void*));
    picture.height = height;
    picture.width = width;
    pixelRowSize = ((24 * width + 31) / 32) * 4;
    
    // bitmap image data read from bottom to top
    for (int i = height - 1; i >= 0; i--) {
        // allocate memory for pixeldata
        picture.rgbValue[i] = (rgb*) malloc(pixelRowSize);
        // read pixeldata from bmp file
        fread(picture.rgbValue[i], 1, pixelRowSize, fptr);
    }
    return picture;
}

// free memory allocated to imagedata
void freeImage(image picture) 
{
    for (int i = picture.height - 1; i >= 0; i--) {
        free(picture.rgbValue[i]);
    }
    free(picture.rgbValue);
}

void BitmapToAsciiArt(char fileName[31])
{
    int fileType;
    char saveFileName[31];

    // user input a name to save ascii image as
    printf("Save as: \n");
    scanf(" %30s", saveFileName);
    
    // open bitmap file
    FILE *fptr = fopen(fileName, "rb"); 

    if (fptr == NULL) {
    printf("Error! Could not open file!");
    exit(1);
    }
   
    // read fileheader data 
    fread(&fileHeader, sizeof(bitmapFileHeader), 1, fptr);
    fileType = fileHeader.fileType;
    
    if (fileType != 0x4d42) {
        printf("Error! Wrong filetype!");
        exit(1);
    }
    
    // read infoheader data
    fread(&infoHeader, sizeof(bitmapInfoHeader), 1, fptr);   
    // skip over header data
    fseek(fptr, fileHeader.bitmapOffset, SEEK_SET);
    // read image data and store data in struct image
    image picture = readImage(fptr, infoHeader.bitmapHeight, infoHeader.bitmapWidth);
    
    // function that converts .bmp file to ascii image
    bmpToAscii(picture, saveFileName);
    
    // close file
    fclose(fptr);
    // free memory 
    freeImage(picture);
}

void programLena() 
{
    int redVal, greenVal, blueVal;
    int rowStart, rowEnd, columnStart, columnEnd;
    char choiceMirrored;
    char choiceSave;
    char choiceSaveMirrored;
    char saveFileName[31];
    
    // read lena.txt file
    readFile("lena.txt");
    makeCharArray();
    
    // user input range 
    printf("Give a range for printing the image in ascii symbols in the following format: rowStart rowEnd columnStart columnEnd\n");
    scanf("%d %d %d %d", &rowStart, &rowEnd, &columnStart, &columnEnd);

    // checking for valid inputs
    while (rowStart < 0 || rowEnd < 0 || columnStart < 0 || columnEnd < 0) {
        printf("Invalid range (no negative numbers). Try another input.\n");
        scanf("%d %d %d %d", &rowStart, &rowEnd, &columnStart, &columnEnd);
    } 
    
    while (rowStart > rowEnd || columnStart > columnEnd) {
        printf("Invalid range, (start bigger than end). Try another input.\n");
        scanf("%d %d %d %d", &rowStart, &rowEnd, &columnStart, &columnEnd);
    } 
    
    while (rowStart > sizeLena || rowEnd > sizeLena || columnStart > sizeLena || columnEnd > sizeLena) {
        printf("Invalid range, (input too large). Try another input.\n");
        scanf("%d %d %d %d", &rowStart, &rowEnd, &columnStart, &columnEnd);
    } 

    // user input to print image in color
    printf("Give color to print ascii image in rgb in following format: redvalue greenvalue bluevalue\n");
    scanf("%d %d %d", &redVal, &greenVal, &blueVal);

    while (redVal < 0 || redVal > 255 || greenVal < 0 || greenVal > 255 || blueVal < 0 || blueVal > 255) {
        printf("Invalid rgb value!, Try another input.");
        scanf("%d %d %d", &redVal, &greenVal, &blueVal);
    } 
    
    // function that prints the average greyscale value
    averageGreyscale(rowStart, rowEnd, columnStart, columnEnd);

    // user input to print image in mirrored or not
    printf("To print image mirrored type \'m\'. Otherwise type character other than \'m\'\n");
    scanf(" %c", &choiceMirrored);

    if (choiceMirrored == 'm') {
        addColour(redVal, greenVal, blueVal);
        printAsciiMirrored(rowStart, rowEnd, columnStart, columnEnd);
        colourReset();

        printf("To save this image type \'s\'. Otherwise type character other than \'s\'\n");
        scanf(" %c", &choiceSaveMirrored);

        if (choiceSaveMirrored == 's') {
            printf("Save as (max 30 characters):");
            scanf("%30s", saveFileName);

            saveImageAsciiMirrored(saveFileName, rowStart, rowEnd, columnStart, columnEnd);
        } else {

        }

    } else {
        addColour(redVal, greenVal, blueVal);
        printAscii(rowStart, rowEnd, columnStart, columnEnd);
        colourReset();
        printf("To save this image type \'s\'. Otherwise type character other than \'s\'\n");
        scanf(" %c", &choiceSave);

        if (choiceSave == 's') {
            printf("Save as (max 30 characters):");
            scanf(" %30s", saveFileName);

            saveImageAscii(saveFileName, rowStart, rowEnd, columnStart, columnEnd);
        } else {

        }
    } 
}

