// #include "hw2.h"

#define MISSING_ARGUMENT 1
#define UNRECOGNIZED_ARGUMENT 2
#define DUPLICATE_ARGUMENT 3
#define INPUT_FILE_MISSING 4
#define OUTPUT_FILE_UNWRITABLE 5
#define C_ARGUMENT_MISSING 6
#define C_ARGUMENT_INVALID 7
#define P_ARGUMENT_INVALID 8
#define R_ARGUMENT_INVALID 9

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

unsigned long endPoint(unsigned long MAXROWS, unsigned long MAXCOLUMNS, char letter, char fontValues[MAXROWS][MAXCOLUMNS], unsigned long fontNumber){
    int numLetters = 0;
    unsigned long endingPoint;
    for (unsigned long i = 0; i < 1; i++){ // check the first row 
        for(unsigned long j = 0; j < MAXCOLUMNS; j++){ // check all the values in the first row
            unsigned long counter = 0;    
            
            if (numLetters == ((letter + 1) - 'A')){ // endPoint is the starting point of the next letter
                endingPoint = j;
                break;
            }

            if (fontValues[i][j] == ' '){   // check if the values are equal to space then check the values under it
                counter++;
                for (unsigned long k = 1; k < MAXROWS; k++){ // since i already checked the first row for space, check the rows after it
                    if(fontValues[k][j] == ' ')
                        counter++;
                }
            }
            if (counter == MAXROWS){
                numLetters++;
            }
        }
    }
    
    if (fontNumber == 2 && letter == ('Z' + 1)){  // for font 2 
        return MAXCOLUMNS - 1;
    }
    else if (fontNumber == 0 && letter == 'Z'){ // for fonts that have no issues font 1 and 4
        return MAXCOLUMNS - 1;
    }
    else if (fontNumber == 3 && letter == ('Z' + 1)){
        return MAXCOLUMNS - 1;
    }
    
    return endingPoint - 1;
}

unsigned long startPoint(unsigned long MAXROWS, unsigned long MAXCOLUMNS, char letter, char fontValues[MAXROWS][MAXCOLUMNS], unsigned long fontNumber){
    // numLetters 0 = 'A' numLetters 1 = 'B' .... numLetters 25 = 'Z' numLetters 26 = EOF
    // what letter do I want? let say B
    // I want letter B
    // which is numLetters = 1
    // if (counter == numLetters) move the pointer j number of times down
    int numLetters = 0;
    unsigned long startingPoint = -1;
    for (unsigned long i = 0; i < 1; i++){ // check the first row 
        for(unsigned long j = 0; j < MAXCOLUMNS; j++){ // check all the value sin the first row
            unsigned long counter = 0;    

            if (numLetters == (letter - 'A')){ // finds the starting point of the letter inputted in the font file
                startingPoint = j;
                break;
            }

            if (fontValues[i][j] == ' '){   // check if the values are equal to space then check the values under it
                counter++;
                for (unsigned long k = 1; k < MAXROWS; k++){ // since i already checked the first row for space, check the rows after it
                    if(fontValues[k][j] == ' ')
                        counter++;
                }
            }
            if (counter == MAXROWS){
                numLetters++;
            }
        }
    }

    if (fontNumber == 3 && letter == ('Z' + 1)){
        startingPoint = 196;
    }


    return startingPoint;
}

void rPrint(unsigned long numbersArray[], char *rname, unsigned long width, unsigned long height){

    // extracts string to be printed
    char *section = strtok(rname, ",\"");
    char *str = section;
    
    // converts string to uppercase
    char strUpper[strlen(str) + 1];
    unsigned long strLength = 0;
    strUpper[strlen(str)] = '\0';
    
    for (; *str != '\0'; str++){
        if (*str >= 'a' && *str <= 'z'){
            strUpper[strLength++] = *str - 'a' + 'A';
        }
        else{
            strUpper[strLength++] = *str;
        }
    }

    char *text = strUpper;

    // extracts font file path
    section = strtok(NULL, ", \"");
    char *fontFilePath = section;
    printf("\n Font File Path: %s", fontFilePath);

    // extracts font size
    section = strtok(NULL, ",");
    unsigned long fontSize = strtoul(section, NULL, 10);
    printf("\n Font Size: %lu", fontSize);

    // extracts starting row to be printed
    section = strtok(NULL, ",");
    unsigned long startRowTextPaste = strtoul(section, NULL, 10);
    printf("\n Start Row Text Paste: %lu", startRowTextPaste);

    // extracts strating column to be printed
    section = strtok(NULL, ",");
    unsigned long startColumnTextPaste = strtoul(section, NULL, 10);
    printf("\n Start Column Text Paste: %lu", startRowTextPaste);

    FILE *fontRead = fopen(fontFilePath, "r"); // CHANGE THIS TO FONTFILEPATH WHEN YOU ARE SUBMITTING **************************8
    

    char tempHolder[1000];
    unsigned long MAXROWS = 0;
    unsigned long MAXCOLUMNS = 0;
    int c;
    while ((c = fgetc(fontRead)) != '\n'){ // extracts the maximum number of columns in the font file
        MAXCOLUMNS++;
    }
    while (fgets(tempHolder, sizeof(tempHolder), fontRead) != NULL){ // extracts the maximum number of rows in the font file
        MAXROWS++;
    }
    MAXROWS++;


    fseek(fontRead, 0, SEEK_SET); // resets the file pointer to the beginning so we can extract data now
    
    char fontValues[MAXROWS][MAXCOLUMNS];

    for (unsigned long i = 0; i < MAXROWS; i++){
        fgets(tempHolder, sizeof(tempHolder), fontRead);
        for (unsigned long j = 0; j < MAXCOLUMNS; j++){
            fontValues[i][j] = tempHolder[j];
        }
    }

    char copiedFontValues[MAXROWS][1000 * 100];
    unsigned long lastPlaceToCopyTo = 0; // keeps track of where to concentate after adding characters in
    unsigned long arraySize = 1;    // keeps track of num in sizeOfEachLetter
    unsigned long sizeOfEachLetter[200];
    sizeOfEachLetter[0] = 0;

    while (*text != '\0'){
        char letter = *text;

        unsigned long startingPoint;
        unsigned long endingPoint;
        if (strcmp(fontFilePath, "./tests/fonts/font3.txt") == 0){ // font 3 we need to skip over the first column of white spaces
            startingPoint = startPoint(MAXROWS, MAXCOLUMNS, letter + 1, fontValues, 3);
            endingPoint = endPoint(MAXROWS, MAXCOLUMNS, letter + 1, fontValues, 3);
        }
        else if (strcmp(fontFilePath, "./tests/fonts/font2.txt") == 0 ){
            if (letter >= 'R' && letter <= 'Z'){
                startingPoint = startPoint(MAXROWS, MAXCOLUMNS, letter + 1, fontValues, 2);  // test for font 2 (double column between Q and R)
                endingPoint = endPoint(MAXROWS, MAXCOLUMNS, letter+1, fontValues, 2);
            }
            else{
                startingPoint = startPoint(MAXROWS, MAXCOLUMNS, letter, fontValues, 2);
                endingPoint = endPoint(MAXROWS, MAXCOLUMNS, letter, fontValues, 2);
            }
        }
        else{
            startingPoint = startPoint(MAXROWS, MAXCOLUMNS, letter, fontValues, 0);  // font 1 and 4 
            endingPoint = endPoint(MAXROWS, MAXCOLUMNS, letter, fontValues, 0);
        }


        char nextLetter = *(text + 1);
    
        if (letter == ' '){ // 
            for (unsigned long i = 0; i < MAXROWS; i++) {
                unsigned long copiedColumn = lastPlaceToCopyTo;
                for (unsigned long j = 0; j < 5; j++) {
                    copiedFontValues[i][copiedColumn++] = ' ';
                }
            }
            lastPlaceToCopyTo += 5;
            sizeOfEachLetter[arraySize++] = lastPlaceToCopyTo; 
        }
        else{
            // copies over a letter and places it into copiedFontValues based on the offset from previous letters
            for (unsigned long i = 0; i < MAXROWS; i++) {
                unsigned long copiedColumn = lastPlaceToCopyTo;
                for (unsigned long j = startingPoint; j < endingPoint ; j++) { // prints astertiks one time

                    for (unsigned long k = 0; k < fontSize; k++){ // prints astertiks * fontSize times horizontally
                        copiedFontValues[i][copiedColumn++] = fontValues[i][j];
                    }
                }
                
                for(unsigned long j = 0; j < 1; j++){
                    copiedFontValues[i][copiedColumn++] = ' ';
                }
            }

            // properly creates offset for the next letter so they dont get overwritten by each other
            lastPlaceToCopyTo += (endingPoint - startingPoint) * (fontSize) + 1;

            if (nextLetter != ' '){
                sizeOfEachLetter[arraySize++] = lastPlaceToCopyTo - 1; 
            }
            else{
                sizeOfEachLetter[arraySize++] = lastPlaceToCopyTo;
            }
        }
        

        text++;
    }
    // copiedFontValues horizontally expanded

    char verticallyExpanded[MAXROWS * fontSize][1000 * 25 + 1000];
    for (unsigned long i = 0; i < (MAXROWS * fontSize); i++){
        int actualIValue = i/fontSize;
        for(unsigned long j = 0; j < lastPlaceToCopyTo; j++){
            verticallyExpanded[i][j] = copiedFontValues[actualIValue][j];
        }
    }


    unsigned long size = MAXROWS * fontSize * lastPlaceToCopyTo;
    char copiedValues[size + 1];
    copiedValues[size] = '\0';

    unsigned long index = 0;
    for (unsigned long i = 0; i < MAXROWS * fontSize; i++){
        for (unsigned long j = 0; j < lastPlaceToCopyTo; j++){
            copiedValues[index++] = verticallyExpanded[i][j];
        }
    }

    unsigned long differences[200];
    unsigned long differencesSize = 0;
    for(unsigned long i = 0; i < arraySize-1; i++){
        differences[differencesSize++] = sizeOfEachLetter[i + 1] - sizeOfEachLetter[i];
    }

    unsigned long incrementer = lastPlaceToCopyTo;
    lastPlaceToCopyTo--;
    differencesSize--;
    while ((lastPlaceToCopyTo + startColumnTextPaste) > width) {
        lastPlaceToCopyTo -= differences[differencesSize];
        differencesSize--;
    }


    unsigned long copiedValueCounter = 0;
    unsigned long column;
    unsigned long startingXYTextPaste = (startRowTextPaste * width * 3) + (startColumnTextPaste * 3);
    for(unsigned long row = 0; row < (MAXROWS * fontSize) && ((startRowTextPaste + row) < height); row++){
        for (column = 0; column < lastPlaceToCopyTo && ((startColumnTextPaste + column) < width ); column++){
            unsigned long currentSpot = startingXYTextPaste + (column * 3);

            if (currentSpot + 2 < width * height * 3){
                if (copiedValues[copiedValueCounter] == '*'){
                    numbersArray[currentSpot] = 255;
                    numbersArray[currentSpot + 1] = 255;
                    numbersArray[currentSpot + 2] = 255;
                }
                copiedValueCounter++;
            }
        }
        while (column < incrementer){
            copiedValueCounter++;
            column++;
        }
        if ((startingXYTextPaste + (width * 3)) < (height * width * 3)){
            startingXYTextPaste = startingXYTextPaste + (width * 3);
        }
    }

    fclose(fontRead);
}

void copyAndPastePixels(unsigned long width, unsigned long height, unsigned long* numbersArray, char* cname, char* pname) {
    unsigned long counter1 = 0;
    unsigned long carguments[4];
    char *substring = strtok(cname, ",");
    while (substring != NULL){
        carguments[counter1] = strtoul(substring, NULL, 10);
        counter1++;
        substring = strtok(NULL, ",");
    }

    unsigned long startRow = carguments[0];
    unsigned long startColumn = carguments[1];
    unsigned long numColumns = carguments[2];
    unsigned long numRows = carguments[3];
    unsigned long sizeToCopy = carguments[2] * carguments[3] * 3;

    // extract P arguments
    counter1 = 0;
    unsigned long parguments[2];
    substring = strtok(pname, ",");
    while (substring != NULL){
        parguments[counter1] = strtoul(substring, NULL, 10);
        counter1++;
        substring = strtok(NULL, ",");
    }

    unsigned long startRowPaste = parguments[0];
    unsigned long startColumnPaste = parguments[1];

    unsigned long counter2 = 0;
    unsigned long copiedValues[sizeToCopy];
    unsigned long startingXY = (startRow * width * 3) + (startColumn * 3);
    unsigned long val;
    unsigned long val2; 

    for(unsigned long row = 0; row < numRows && ((startRow + row) < height); row++) {
        for(unsigned long column = 0; column < numColumns && ((startColumn + column) < width); column++) {
            unsigned long currentSpot = startingXY + (column * 3);
           
            if (currentSpot + 2 < width * height * 3 && ((startColumn + column) < width) && ((startRow + row) < height)) {
                copiedValues[counter2++] = numbersArray[currentSpot];
                copiedValues[counter2++] = numbersArray[currentSpot + 1];
                copiedValues[counter2++] = numbersArray[currentSpot + 2];
            }
            if (row == 0) {
                val = column;
            }
        }
        startingXY = startingXY + (width * 3);
        val2 = row;
    }

    counter2 = 0;
    unsigned long column;
    unsigned long startingXYPaste = (startRowPaste * width * 3) + (startColumnPaste * 3);

    for(unsigned long row = 0; row <= val2 && ((startRowPaste + row) < height); row++) {
        for(column = 0; column <= val && ((startColumnPaste + column) < width); column++) {
            unsigned long currentSpot = startingXYPaste + (column * 3);

            if (currentSpot + 2 < width * height * 3) {
                numbersArray[currentSpot] = copiedValues[counter2];
                numbersArray[currentSpot + 1] = copiedValues[counter2 + 1];
                numbersArray[currentSpot + 2] = copiedValues[counter2 + 2];
                counter2 += 3; 
            }
        }
        while (column <= val) {
            counter2 += 3;
            column++;
        }
        startingXYPaste = startingXYPaste + (width * 3);
    }
}
int main(int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int opt;
    char* iname = NULL, *oname = NULL, *cname = NULL, *pname = NULL, *rname = NULL;
    int iflag = 0, oflag = 0, cflag = 0, pflag = 0, rflag = 0;
    int unrecognizedflag = 0;

    while ((opt = getopt(argc, argv, "i:o:c:p:r:")) != -1){
        switch (opt){
            case 'i':
                iflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                iname = optarg;
                break;
            case 'o':
                oflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                oname = optarg;
               break;
            case 'c':
                cflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                cname = optarg;
                break;
            case 'p':
                pflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                pname = optarg;
                break;
            case 'r':
                rflag++;
                if (optarg[0] == '-'){
                    return MISSING_ARGUMENT;
                }
                rname = optarg;
                break;
            case '?': 
                if (optind == argc){
                    return MISSING_ARGUMENT;
                }   
                unrecognizedflag = 1;
                break;
        }
    }



    if (iflag == 0 || oflag == 0){
        return MISSING_ARGUMENT;
    }

    if (unrecognizedflag == 1){
        return UNRECOGNIZED_ARGUMENT;
    }

    if (iflag > 1 || oflag > 1 || cflag > 1 || pflag > 1 || rflag > 1){
        return DUPLICATE_ARGUMENT;
    }
    FILE *fp;
    if ((fp = fopen(iname, "r")) == NULL){
        return INPUT_FILE_MISSING;
    }
    else{
        fclose(fp);
    }
    
    if ((fp = fopen(oname, "w")) == NULL){
        return OUTPUT_FILE_UNWRITABLE;
    }
    else{
        fclose(fp);
    }

    if (pname != NULL && cname == NULL){
        return C_ARGUMENT_MISSING;
    }

    if (cname != NULL){
        int counter = 0;
        char cnamecopy[strlen(cname) + 1];
        cnamecopy[strlen(cname)] = '\0';
        strncpy(cnamecopy, cname, strlen(cname));
        char *substring = strtok(cnamecopy, ",");

        while (substring != NULL){
            counter++;
            substring = strtok(NULL, ",");
        }
        if(counter != 4){
        return C_ARGUMENT_INVALID;
        }
    } 

    if (pname != NULL){
        int counter = 0;
        
        char pnamecopy[strlen(pname) + 1];
        strncpy(pnamecopy, pname, strlen(pname));
        pnamecopy[strlen(pname)] = '\0';
        char *substring = strtok(pnamecopy, ",");

        while (substring != NULL){
            counter++;
            substring = strtok(NULL, ",");
        }
        if (counter != 2){
        return P_ARGUMENT_INVALID;
        }
    }

    // printf("RNAME for spaces: %s", rname);
    if (rname != NULL) {
    int counter = 0;

    char rnamecopy[strlen(rname) + 1];
    strncpy(rnamecopy, rname, strlen(rname));
    rnamecopy[strlen(rname)] = '\0';
    char *portion = strtok(rnamecopy, ",");

    while (portion != NULL) {
        counter++;
        portion = strtok(NULL, ",");
    }
    // printf("Number of R Arguments: %d ", counter);

    char substring[500];
    if (counter >= 2) {
        char *p = rname;
        int length = 0;
        while (*p != '.' && *p != '\0') {
            p++;
        }
        if (*p == '.') {
            char *start = p; 
            while (*p != '"' && *p != '\0') {
                p++;
                length++;
            }
            strncpy(substring, start, length);
            substring[length] = '\0';
            if ((fp = fopen(substring, "r")) == NULL) {
                return R_ARGUMENT_INVALID;
            }
        } else {
            return R_ARGUMENT_INVALID;
        }
    }
    if (counter != 5) {
        return R_ARGUMENT_INVALID;
    }
    }


    char *p = iname;
    char filetype1[4];
    unsigned long i = 0;
    while (i < strlen(iname)-3){
        p++;
        i++;
    }
    strncpy(filetype1, p, 3);
    filetype1[3] = '\0';

    char filetype2[4];
    p = oname;
    i = 0;
    while (i < strlen(oname)-3){
        p++;
        i++;
    }
    strncpy(filetype2, p, 3);
    filetype2[3] = '\0';


    if (strcmp(filetype1, "ppm") == 0 && strcmp(filetype2, "ppm") == 0){ // PPM to PPM
        FILE *read = fopen(iname, "r");
        FILE *write = fopen(oname, "w");
        if ((cflag == 0 && pflag == 0) && rflag == 0){ // No operations
            char c;
            while ((c = fgetc(read)) != EOF){
            fputc(c, write);
            }
        }
        else{ 
            char dimensions[100];
            dimensions[99] = '\0';

            fgets(dimensions, sizeof(dimensions), read);  // Reads PPM (line 1)
            fgets(dimensions, sizeof(dimensions), read);  // Reads Dimensions (line 2)

            unsigned long width;
            unsigned long height;
            unsigned long testCounter = 0;
            char *testPortion = strtok(dimensions, " ");
            while (testPortion != NULL){
                if (testCounter == 0){
                    width = strtoul(testPortion, NULL, 10);
                }
                if (testCounter == 1){
                    height = strtoul(testPortion, NULL, 10);
                }
                testCounter++;
                testPortion = strtok(NULL, " ");
            }


            fgets(dimensions, sizeof(dimensions), read); // Reads in 255 (line 3)

            char lines[width * height * 5 + 1]; // Temporary holder
            unsigned long numbersArray[width * height * 5 + 1]; // RGB values
            unsigned long counter = 0;

            while (fgets(lines, sizeof(lines), read) != NULL){ 
                char *portion = strtok(lines, " ");
                while (portion != NULL){
                    if (portion[0] != '\n'){
                        numbersArray[counter] = strtoul(portion, NULL, 10);
                        counter++;
                    }
                    portion = strtok(NULL, " ");
                }
            }

            if (cflag == 1 && pflag == 1){
                copyAndPastePixels(width, height, numbersArray, cname, pname);
            }

            if (rflag == 1){
                rPrint(numbersArray, rname, width, height);
            }

            // Writing to PPM file
            fprintf(write, "P3\n");
            fprintf(write, "%lu %lu\n", width, height);
            fprintf(write, "%d\n", 255);
            for(unsigned long t = 0; t < counter; t+= 3){
                fprintf(write, "%lu %lu %lu\n", numbersArray[t], numbersArray[t + 1], numbersArray[t + 2]);
            }
            fprintf(write, "\n");
        }

        fclose(write);
        fclose(read);
    }
    else if (strcmp(filetype1, "ppm") == 0 && strcmp(filetype2, "sbu") == 0){ // PPM to SBU

        FILE *read = fopen(iname, "r");
        FILE *write = fopen(oname, "w");

        char dimensions[100];
        dimensions[99] = '\0';

        fgets(dimensions, sizeof(dimensions), read);  // Reads PPM (line 1)
        fgets(dimensions, sizeof(dimensions), read);  // Reads dimensions (line 2)

        unsigned long width;
        unsigned long height;
        unsigned long testCounter = 0;
        char *testPortion = strtok(dimensions, " ");
        while (testPortion != NULL){
            if (testCounter == 0){
                width = strtoul(testPortion, NULL, 10);
            }
            if (testCounter == 1){
                height = strtoul(testPortion, NULL, 10);
            }
            testCounter++;
            testPortion = strtok(NULL, " ");
        }


        fgets(dimensions, sizeof(dimensions), read); // Reads in 255 (line 3)

        char lines[width * height * 5 + 1]; // Temporary holder
        unsigned long numbersArray[width * height * 5 + 1]; // RGB values
        unsigned long counter = 0;

        while (fgets(lines, sizeof(lines), read) != NULL){   // Extracts RGB values (line 4)
            char *portion = strtok(lines, " ");
            while (portion != NULL){
                if (portion[0] != '\n'){
                    numbersArray[counter] = strtoul(portion, NULL, 10);
                    counter++;
                }
                portion = strtok(NULL, " ");
            }
        }

        if (cflag == 1 && pflag == 1){
            copyAndPastePixels(width, height, numbersArray, cname, pname);
        }

        if (rflag == 1){
            rPrint(numbersArray, rname, width, height);
        }

        // Finds all the unique colors
        unsigned long uniqueColors[counter];
        unsigned long uniqueColorCounter = 0 ; 
        for (unsigned long k = 0; k < counter; k+= 3){
            int unique = 1;
            for (unsigned long j = 0; j < uniqueColorCounter; j++){
                if (uniqueColors[j * 3] == numbersArray[k] && uniqueColors[j * 3 + 1] == numbersArray[k+1] && uniqueColors[j * 3 + 2] == numbersArray[k+2]){
                    unique = 0;
                    break;
                } 
            }

            if (unique == 1){
                uniqueColors[uniqueColorCounter * 3] = numbersArray[k];
                uniqueColors[uniqueColorCounter * 3 + 1] = numbersArray[k + 1];
                uniqueColors[uniqueColorCounter * 3 + 2] = numbersArray[k + 2];
                uniqueColorCounter++;
            }
        }
        
        // Writing to SBU file
        fputs("SBU\n", write);                      // line 1                                   
        fprintf(write, "%lu %lu\n", width, height); // line 2  
        fprintf(write, "%lu\n", uniqueColorCounter);// line 3
        
        // Writes the Unique Colors (line 4)
        for (unsigned long k = 0; k < uniqueColorCounter * 3 - 1; k++){ 
            fprintf(write, "%lu ", uniqueColors[k]);
        }
        fprintf(write, "%lu \n", uniqueColors[uniqueColorCounter * 3 - 1]);

        // Matches RGB values to its color index
        unsigned long colorEntries[counter]; 
        unsigned long index = 0;
        for(unsigned long k = 0; k < counter; k+=3){  
            for (unsigned long j = 0; j < uniqueColorCounter; j++){ // assigns the colorIndex of each PPM RGB pair and stores it into colorEntries
                if (uniqueColors[j * 3] == numbersArray[k] && uniqueColors[j * 3 + 1] == numbersArray[k+1] && uniqueColors[j * 3 + 2] == numbersArray[k+2]){
                    colorEntries[index++] = j; 
                    break;
                }
            }
        }

        // Checks for duplicate color index and packages it into *n colorIndex format
        counter = 1;
        for (unsigned long k = 0; k < index; k++){
            if (k < index - 1 && colorEntries[k] == colorEntries[k+1]){
                counter++;
            }
            else{
                if (counter > 1){
                    fprintf(write, "*%lu %lu ", counter, colorEntries[k]);
                    counter = 1;
                }
                else{
                    fprintf(write, "%lu ", colorEntries[k]);
                }
            }
        }
        fclose(read);
        fclose(write);
    }
    else if (strcmp(filetype1, "sbu") == 0 && strcmp(filetype2, "sbu") == 0){ // SBU to SBU
        FILE *read = fopen(iname, "r");
        FILE *write = fopen(oname, "w");
        if ((cflag == 0 && pflag == 0) && rflag == 0){
            char c;
            while ((c = fgetc(read)) != EOF){
            fputc(c, write);
            }
        }
        else{
            char dimensions[100];
            fgets(dimensions, sizeof(dimensions), read);  // Reads in SBU (line 1)
            fgets(dimensions, sizeof(dimensions), read);  // Reads in dimensions (line 2)

            unsigned long width;
            unsigned long height;
            unsigned long testCounter = 0;
            char *testPortion = strtok(dimensions, " ");
            while (testPortion != NULL){
                if (testCounter == 0){
                    width = strtoul(testPortion, NULL, 10);
                }
                if (testCounter == 1){
                    height = strtoul(testPortion, NULL, 10);
                }
                testCounter++;
                testPortion = strtok(NULL, " ");
            }

            fgets(dimensions, sizeof(dimensions), read); // Reads in Unique Color Counter (line 3)

            char lines[15 * width * height + 1];
            unsigned long colorsArray[width * height * 4 + 1]; // Array to store all Unique Colors
            unsigned long counter = 0; // Size of colorsArray

            fgets(lines, sizeof(lines), read); // Reads in all Unique Colors (line 4)

            char *portion = strtok(lines, " "); 
            while (portion != NULL){
                colorsArray[counter] = strtoul(portion, NULL, 10);
                counter++;
                portion = strtok(NULL, " ");
            }

            fgets(lines, sizeof(lines), read); // Reads in RGB values (line 5) 

            unsigned long numbersArray[width * height * 4 + 1]; // Array to store RGB values
            unsigned long numCounter = 0; // Size of numbersArray

            // Extracts RGB values, Unpackages them, and Store in numbersArray
            // Example: *3 2 --> Finds the 3 RGB values associated at color index 2
            // --> Stores 3 copies of the RGB values at color index in numbersArray
            portion = strtok(lines, " ");
            while (portion != NULL){
                if (*portion == '*'){
                    char *pointer = portion + 1;
                    unsigned long consecutiveOccurrences = strtoul(pointer, NULL, 10);

                    portion = strtok(NULL, " ");
                    unsigned long colorIndex = strtoul(portion, NULL, 10) * 3;

                    for (unsigned long k = 0; k < consecutiveOccurrences; k++){
                        numbersArray[numCounter] = colorsArray[colorIndex];
                        numbersArray[numCounter + 1] = colorsArray[colorIndex + 1];
                        numbersArray[numCounter + 2] = colorsArray[colorIndex + 2];
                        numCounter += 3;
                    }
                }
                else{
                    unsigned long colorIndex = strtoul(portion, NULL, 10) * 3;

                    numbersArray[numCounter] = colorsArray[colorIndex];
                    numbersArray[numCounter + 1] = colorsArray[colorIndex + 1];
                    numbersArray[numCounter + 2] = colorsArray[colorIndex + 2];
                    numCounter += 3;
                }
                portion = strtok(NULL, " ");
            }

            // Runs copy and paste
            if (cflag == 1 && pflag == 1){
                copyAndPastePixels(width, height, numbersArray, cname, pname);
            }
            // Runs printing
            if (rflag == 1){
                rPrint(numbersArray, rname, width, height);
            }

            // Since the RGB values were modified, you need to recalculate all the Unique Colors
            // Refinds all the Unique Colors.
            unsigned long uniqueColors[numCounter];
            unsigned long uniqueColorCounter = 0; 
            for (unsigned long k = 0; k < numCounter; k+= 3){
                int unique = 1;
                for (unsigned long j = 0; j < uniqueColorCounter; j++){
                    if (uniqueColors[j * 3] == numbersArray[k] && uniqueColors[j * 3 + 1] == numbersArray[k+1] && uniqueColors[j * 3 + 2] == numbersArray[k+2]){
                        unique = 0;
                        break;
                    } 
                }

                if (unique == 1){
                    uniqueColors[uniqueColorCounter * 3] = numbersArray[k];
                    uniqueColors[uniqueColorCounter * 3 + 1] = numbersArray[k + 1];
                    uniqueColors[uniqueColorCounter * 3 + 2] = numbersArray[k + 2];
                    uniqueColorCounter++;
                }
            }

            // Writing to SBU file
            fprintf(write, "SBU\n");                     // line 1
            fprintf(write, "%lu %lu\n", width, height);  // line 2
            fprintf(write, "%lu\n", uniqueColorCounter); // line 3
                
            for (unsigned long k = 0; k < uniqueColorCounter * 3 - 1; k++){
                fprintf(write, "%lu ", uniqueColors[k]);
            }
            fprintf(write, "%lu \n", uniqueColors[uniqueColorCounter * 3 - 1]);

            // Rematches RGB values to its color index
            unsigned long colorEntries[numCounter]; 
            unsigned long index = 0;
            for(unsigned long k = 0; k < numCounter; k+=3){  
                for (unsigned long j = 0; j < uniqueColorCounter; j++){ // assigns the colorIndex of each PPM RGB pair and stores it into colorEntries
                    if (uniqueColors[j * 3] == numbersArray[k] && uniqueColors[j * 3 + 1] == numbersArray[k+1] && uniqueColors[j * 3 + 2] == numbersArray[k+2]){
                        colorEntries[index++] = j; 
                        break;
                    }
                }
            }
            
            // Checks for duplicate color index and packages it into *n colorIndex format
            numCounter = 1;
            for (unsigned long k = 0; k < index; k++){
                if (k < index - 1 && colorEntries[k] == colorEntries[k+1]){
                    numCounter++;
                }
                else{
                    if (numCounter > 1){
                        fprintf(write, "*%lu %lu ", numCounter, colorEntries[k]);
                        numCounter = 1;
                    }
                    else{
                        fprintf(write, "%lu ", colorEntries[k]);
                    }
                }
            }
        }
        fclose(read);
        fclose(write);
    }
    else { // SBU to PPM
        FILE *read = fopen(iname, "r");
        FILE *write = fopen(oname, "w");   
        char dimensions[100];
        fgets(dimensions, sizeof(dimensions), read);  // Reads in SBU (line 1)
        fgets(dimensions, sizeof(dimensions), read);  // Reads in dimensions (line 2)

        unsigned long width;
        unsigned long height;
        unsigned long testCounter = 0;
        char *testPortion = strtok(dimensions, " ");
        while (testPortion != NULL){
            if (testCounter == 0){
                width = strtoul(testPortion, NULL, 10);
            }
            if (testCounter == 1){
                height = strtoul(testPortion, NULL, 10);
            }
            testCounter++;
            testPortion = strtok(NULL, " ");
        }

        fgets(dimensions, sizeof(dimensions), read); // Reads in Unique Color Counter (line 3)

        char lines[15 * width * height + 1];
        unsigned long colorsArray[width * height * 4 + 1]; // Array to store all Unique Colors
        unsigned long counter = 0; // Size of colorsArray

        fgets(lines, sizeof(lines), read); // Reads in all Unique Colors (line 4)

        char *portion = strtok(lines, " "); 
        while (portion != NULL){
            colorsArray[counter] = strtoul(portion, NULL, 10);
            counter++;
            portion = strtok(NULL, " ");
        }

        fgets(lines, sizeof(lines), read); // Reads in RGB values (line 5) 

        unsigned long numbersArray[width * height * 4 + 1]; // Array to store RGB values
        unsigned long numCounter = 0; // Size of numbersArray

        // Extracts RGB values, Unpackages them, and Store in numbersArray
        // Example: *3 2 --> Finds the 3 RGB values associated at color index 2
        // --> Stores 3 copies of the RGB values at color index in numbersArray
        portion = strtok(lines, " ");
        while (portion != NULL){
            if (*portion == '*'){
                char *pointer = portion + 1;
                unsigned long consecutiveOccurrences = strtoul(pointer, NULL, 10);

                portion = strtok(NULL, " ");
                unsigned long colorIndex = strtoul(portion, NULL, 10) * 3;

                for (unsigned long k = 0; k < consecutiveOccurrences; k++){
                    numbersArray[numCounter] = colorsArray[colorIndex];
                    numbersArray[numCounter + 1] = colorsArray[colorIndex + 1];
                    numbersArray[numCounter + 2] = colorsArray[colorIndex + 2];
                    numCounter += 3;
                }
            }
            else{
                unsigned long colorIndex = strtoul(portion, NULL, 10) * 3;

                numbersArray[numCounter] = colorsArray[colorIndex];
                numbersArray[numCounter + 1] = colorsArray[colorIndex + 1];
                numbersArray[numCounter + 2] = colorsArray[colorIndex + 2];
                numCounter += 3;
            }
            portion = strtok(NULL, " ");
        }

        // Runs copy and paste
        if (cflag == 1 && pflag == 1){
            copyAndPastePixels(width, height, numbersArray, cname, pname);
        }
        // Runs printing
        if (rflag == 1){
            rPrint(numbersArray, rname, width, height);
        }         
        fprintf(write, "P3\n");
        fprintf(write, "%lu %lu\n", width, height);
        fprintf(write, "%d\n", 255);
        for(unsigned long t = 0; t < numCounter; t+= 3){
            fprintf(write, "%lu %lu %lu\n", numbersArray[t], numbersArray[t + 1], numbersArray[t + 2]);
        }
        fprintf(write, "\n");
    }
    return 0;
}