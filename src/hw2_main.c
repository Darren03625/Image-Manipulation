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
    
    fp = fopen(oname, "w");
    if (fp == NULL){
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
        strncpy(cnamecopy, cname, strlen(cname));
        char *substring = strtok(cnamecopy, ",");

        while (substring != NULL){
            if (strtol(substring, NULL, 10) > 0){
                counter++;
            }
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
        char *substring = strtok(pnamecopy, ",");

        while (substring != NULL){
            counter++;
            substring = strtok(NULL, ",");
        }

        if (counter != 2){
        return P_ARGUMENT_INVALID;
        }
    }

    if (rname != NULL){
        int counter = 0;
        char *portion = strtok(rname, ",");

        while (portion != NULL){
            counter++;
            portion = strtok(NULL, ",");
        }

        if (counter >= 2){
            char* p = rname;
            int length = 0;
            while (*rname != '.' && *rname != '\0'){
                p++;
                rname++;
            }
            while(*p != '"' && *p != '\0'){
                p++;
                length++;
            }
            char substring[length+1];
            strncpy(substring, rname, length);
            substring[length] = '\0';
            if ((fp = fopen(substring, "r")) == NULL){
                return R_ARGUMENT_INVALID;
            }
        }
        if (counter != 5){
        return R_ARGUMENT_INVALID;
        }
    }

    char *p = iname;
    char filetype1[4];
    long unsigned int i = 0;
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



    if (strcmp(filetype1, filetype2) == 0){
        FILE *read = fopen(iname, "r");
        if (strcmp(filetype1, "ppm") == 0 && strcmp(filetype2, "ppm") == 0){ //ppm to ppm
            FILE *write = fopen(oname, "w");

            char dimensions[100];
            fgets(dimensions, sizeof(dimensions), read);  // reads in the PPM, used to skip a line 
            fgets(dimensions, sizeof(dimensions), read);  // reads in the dimensions and overwrites PPM

            unsigned long width = strtoul(strtok(dimensions, " "), NULL, 10); 
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10); 

            fgets(dimensions, sizeof(dimensions), read); // used to skip the line "255" in PPM file

            char lines[width * height * 3 + 1];  // temporary holder for values;
            unsigned long numbersArray[5 * width * height + 1];
            unsigned long counter = 0;
            while (fgets(lines, sizeof(lines), read) != NULL){  // gets each line as a string and stores in the array lines (line 4, 5, 6, ... in PPM)
            // splits the line using strtok using " " and store each number in numbersArray by parsing it
                char *portion = strtok(lines, " ");
                while (portion != NULL){
                    if (strcmp(portion, "\n") != 0){
                        numbersArray[counter] = strtoul(portion, NULL, 10);
                        counter++;
                    }
                    portion = strtok(NULL, " ");   
                }
            }
            if (cflag == 1 && pflag == 1){ 
                // extract C arguments
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
                unsigned long val; // num elements to paste in the row
                unsigned long val2; 
                for(unsigned long row = 0; row < numRows && ((startRow + row) < height); row++){
                    for(unsigned long column = 0; column < numColumns && ((startColumn + column) < width); column++){
                        unsigned long currentSpot = startingXY + (column * 3);

                        if (currentSpot + 2 < width * height * 3 && ((startColumn + column) < width) && ((startRow + row) < height)){
                            copiedValues[counter2++] = numbersArray[currentSpot];
                            copiedValues[counter2++] = numbersArray[currentSpot + 1];
                            copiedValues[counter2++] = numbersArray[currentSpot + 2];
                        }
                        if (row == 0){
                            val = column;
                        }
                    }
                    if ((startingXY + (width * 3 )) < (height * width * 3)){
                        startingXY = startingXY + (width * 3);
                    }
                    val2 = row;
                }

              
                counter2 = 0;
                unsigned long column;
                unsigned long startingXYPaste = (startRowPaste * width * 3) + (startColumnPaste * 3);
                for(unsigned long row = 0; row <= val2 && ((startRowPaste + row) < height); row++){
                    for(column = 0; column <= val && ((startColumnPaste + column) < width); column++){
                        unsigned long currentSpot = startingXYPaste + (column * 3);

                        if (currentSpot + 2 < width * height * 3){
                            numbersArray[currentSpot] = copiedValues[counter2];
                            numbersArray[currentSpot + 1] = copiedValues[counter2 + 1];
                            numbersArray[currentSpot + 2] = copiedValues[counter2 + 2];
                            counter2 +=3; 
                        }
                    }
                    while (column <= val){ // if column <= val means we cant paste anymore in bc its going to go out of bounds
                        counter2 += 3;  // therefore discard the rest of the values in the row in copiedValues
                        column++;
                    }
                    if ((startingXYPaste + (width * 3)) < (height * width * 3)){
                        startingXYPaste = startingXYPaste + (width * 3);
                    }
                }
            }
            
            fprintf(write, "P3\n");
            fprintf(write, "%lu %lu\n", width, height);
            fprintf(write, "%d\n", 255);
            for(unsigned long t = 0; t < counter; t+= 3){
                fprintf(write, "%lu %lu %lu\n", numbersArray[t], numbersArray[t + 1], numbersArray[t + 2]);
            }
            fprintf(write, "\n");
            fclose(write);
            
        }
        else{
            FILE *write = fopen(oname, "w");
            char dimensions[100];
            fgets(dimensions, sizeof(dimensions), read);  // used to skip the SBU header (line 1)
            fgets(dimensions, sizeof(dimensions), read);  // used to read dimensions (line 2)
            
            unsigned long width = strtoul(strtok(dimensions," "), NULL, 10);  // splits the string dimension by " ", extracts width
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10);  // splits the string dimension by " ", extracts height

            fgets(dimensions, sizeof(dimensions), read); // used to read the # unique colors (line 3)

            unsigned long numUniqueColors = strtoul(strtok(dimensions, " "), NULL, 10);
            
            fprintf(write, "SBU\n");
            fprintf(write, "%lu %lu\n", width, height);

            if (cflag == 1 && pflag == 1){
                char lines[15 * width * height + 1];
                unsigned long colorsArray[width * height * 4 + 1]; // stores all the unique colors (line 4)
                unsigned long counter = 0; // size of colorsArray

                fgets(lines, sizeof(lines), read); // used to read the unique color values and store in lines (line 4)

                char *portion = strtok(lines, " "); 
                while (portion != NULL){ // parses the unique color values and store it in colorsArray 
                    colorsArray[counter] = strtoul(portion, NULL, 10);
                    counter++;
                    portion = strtok(NULL, " ");
                }

                fgets(lines, sizeof(lines), read); // used to read the line with color value inputs (line 5) 

                unsigned long numbersArray[width * height * 4 + 1];
                unsigned long numCounter = 0;
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

                // extract C arguments
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
                unsigned long val; // num elements to paste in the row
                unsigned long val2; 
                for(unsigned long row = 0; row < numRows && ((startRow + row) < height); row++){
                    for(unsigned long column = 0; column < numColumns && ((startColumn + column) < width); column++){
                        unsigned long currentSpot = startingXY + (column * 3);

                        if (currentSpot + 2 < width * height * 3 && ((startColumn + column) < width) && ((startRow + row) < height)){
                            copiedValues[counter2++] = numbersArray[currentSpot];
                            copiedValues[counter2++] = numbersArray[currentSpot + 1];
                            copiedValues[counter2++] = numbersArray[currentSpot + 2];
                        }
                        if (row == 0){
                            val = column;
                        }
                    }
                    if ((startingXY + (width * 3 )) < (height * width * 3)){
                        startingXY = startingXY + (width * 3);
                    }
                    val2 = row;
                }

                
                counter2 = 0;
                unsigned long column;
                unsigned long startingXYPaste = (startRowPaste * width * 3) + (startColumnPaste * 3);
                for(unsigned long row = 0; row <= val2 && ((startRowPaste + row) < height); row++){
                    for(column = 0; column <= val && ((startColumnPaste + column) < width); column++){
                        unsigned long currentSpot = startingXYPaste + (column * 3);

                        if (currentSpot + 2 < width * height * 3){
                            numbersArray[currentSpot] = copiedValues[counter2];
                            numbersArray[currentSpot + 1] = copiedValues[counter2 + 1];
                            numbersArray[currentSpot + 2] = copiedValues[counter2 + 2];
                            counter2 +=3; 
                        }
                    }
                    while (column <= val){ // if column <= val means we cant paste anymore in bc its going to go out of bounds
                        counter2 += 3;  // therefore discard the rest of the values in the row in copiedValues
                        column++;
                    }
                    if ((startingXYPaste + (width * 3)) < (height * width * 3)){
                        startingXYPaste = startingXYPaste + (width * 3);
                    }
                }
                unsigned long uniqueColors[numCounter];
                unsigned long uniqueColorCounter = 0; // test if the colors are unique, if unique then add into the uniquecolor array
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

                fprintf(write, "%lu\n", uniqueColorCounter); // writes # unique colors to file             
                    
                for (unsigned long k = 0; k < uniqueColorCounter * 3 - 1; k++){ // writes all the color codes of unique colors
                    fprintf(write, "%lu ", uniqueColors[k]);
                }

                fprintf(write, "%lu \n", uniqueColors[uniqueColorCounter * 3 - 1]);

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
            else{
                fprintf(write, "%lu\n", numUniqueColors);
                char c;
                while ((c = fgetc(read)) != EOF){
                    fputc(c, write);
                } 
            }
            fclose(write);
        }
        fclose(read);
    }
    else{
        FILE *read = fopen(iname, "r");  // read from input file
    
        if (strcmp(filetype1, "ppm") == 0 && strcmp(filetype2, "sbu") == 0){ // this means input file is ppm, and output file is sbu, we need to convert input file from ppm to sbu
            FILE *write = fopen(oname, "w"); // write to a new output file
            
            char dimensions[100];
            fgets(dimensions, sizeof(dimensions), read);  // reads in the PPM, used to skip a line 
            fgets(dimensions, sizeof(dimensions), read);  // reads in the dimensions and overwrites PPM

            unsigned long width = strtoul(strtok(dimensions, " "), NULL, 10); 
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10); 

            fgets(dimensions, sizeof(dimensions), read); // used to skip the line "255" in PPM file

            char lines[width * height + 1];  // temporary holder for values;
            unsigned long numbersArray[5 * width * height + 1];
            unsigned long counter = 0;
            while (fgets(lines, sizeof(lines), read) != NULL){  // gets each line as a string and stores in the array lines (line 4, 5, 6, ... in PPM)
            // splits the line using strtok using " " and store each number in numbersArray by parsing it
                char *portion = strtok(lines, " ");
                while (portion != NULL){
                    if (strcmp(portion, "\n") != 0){
                        numbersArray[counter] = strtoul(portion, NULL, 10);
                        counter++;
                    }
                    portion = strtok(NULL, " ");   
                }
            }

             if (cflag == 1 && pflag == 1){ 
                // extract C arguments
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
                unsigned long val; // num elements to paste in the row
                unsigned long val2; 
                for(unsigned long row = 0; row < numRows && ((startRow + row) < height); row++){
                    for(unsigned long column = 0; column < numColumns && ((startColumn + column) < width); column++){
                        unsigned long currentSpot = startingXY + (column * 3);

                        if (currentSpot + 2 < width * height * 3 && ((startColumn + column) < width) && ((startRow + row) < height)){
                            copiedValues[counter2++] = numbersArray[currentSpot];
                            copiedValues[counter2++] = numbersArray[currentSpot + 1];
                            copiedValues[counter2++] = numbersArray[currentSpot + 2];
                        }
                        if (row == 0){
                            val = column;
                        }
                    }
                    if ((startingXY + (width * 3 )) < (height * width * 3)){
                        startingXY = startingXY + (width * 3);
                    }
                    val2 = row;
                }

              
                counter2 = 0;
                unsigned long column;
                unsigned long startingXYPaste = (startRowPaste * width * 3) + (startColumnPaste * 3);
                for(unsigned long row = 0; row <= val2 && ((startRowPaste + row) < height); row++){
                    for(column = 0; column <= val && ((startColumnPaste + column) < width); column++){
                        unsigned long currentSpot = startingXYPaste + (column * 3);

                        if (currentSpot + 2 < width * height * 3){
                            numbersArray[currentSpot] = copiedValues[counter2];
                            numbersArray[currentSpot + 1] = copiedValues[counter2 + 1];
                            numbersArray[currentSpot + 2] = copiedValues[counter2 + 2];
                            counter2 +=3; 
                        }
                    }
                    while (column <= val){ // if column <= val means we cant paste anymore in bc its going to go out of bounds
                        counter2 += 3;  // therefore discard the rest of the values in the row in copiedValues
                        column++;
                    }
                    if ((startingXYPaste + (width * 3)) < (height * width * 3)){
                        startingXYPaste = startingXYPaste + (width * 3);
                    }
                }
            }

            unsigned long uniqueColors[counter];
            unsigned long uniqueColorCounter = 0; // test if the colors are unique, if unique then add into the uniquecolor array
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
            
            fputs("SBU\n", write);  // writes SBU to the file                                             SBU
            fprintf(write, "%lu %lu\n", width, height);  // writes dimensions to the file                 width height
            fprintf(write, "%lu\n", uniqueColorCounter); // writes # unique colors to file                #
            
            for (unsigned long k = 0; k < uniqueColorCounter * 3 - 1; k++){ // writes all the color codes of unique colors
                fprintf(write, "%lu ", uniqueColors[k]);
            }

            fprintf(write, "%lu \n", uniqueColors[uniqueColorCounter * 3 - 1]);

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

            fclose(write);
            fclose(read);

        }
        else{ // this means we need to convert the input file from sbu to output ppm
            FILE *write = fopen(oname, "w");

            char dimensions[100];
            fgets(dimensions, sizeof(dimensions), read);  // used to skip the SBU header (line 1)
            fgets(dimensions, sizeof(dimensions), read);  // used to read dimensions (line 2)
            
            unsigned long width = strtoul(strtok(dimensions," "), NULL, 10);  // splits the string dimension by " ", extracts width
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10);  // splits the string dimension by " ", extracts height

            fgets(dimensions, sizeof(dimensions), read); // used to read the # unique colors (line 3)

            fprintf(write, "P3\n");
            fprintf(write, "%lu %lu\n", width, height);
            fprintf(write, "%d\n", 255);

            char lines[15 * width * height + 1];
            unsigned long colorsArray[width * height * 4 + 1]; // stores all the unique colors (line 4)
            unsigned long counter = 0; // size of colorsArray

            fgets(lines, sizeof(lines), read); // used to read the unique color values and store in lines (line 4)

            char *portion = strtok(lines, " "); 
            while (portion != NULL){ // parses the unique color values and store it in colorsArray 
                colorsArray[counter] = strtoul(portion, NULL, 10);
                counter++;
                portion = strtok(NULL, " ");
            }

            fgets(lines, sizeof(lines), read); // used to read the line with color value inputs (line 5) 

            if (cflag == 1 && pflag == 1){
                unsigned long numbersArray[width * height * 4 + 1];
                unsigned long numCounter = 0;
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
                // extract C arguments
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
                unsigned long val; // num elements to paste in the row
                unsigned long val2; 
                for(unsigned long row = 0; row < numRows && ((startRow + row) < height); row++){
                    for(unsigned long column = 0; column < numColumns && ((startColumn + column) < width); column++){
                        unsigned long currentSpot = startingXY + (column * 3);

                        if (currentSpot + 2 < width * height * 3 && ((startColumn + column) < width) && ((startRow + row) < height)){
                            copiedValues[counter2++] = numbersArray[currentSpot];
                            copiedValues[counter2++] = numbersArray[currentSpot + 1];
                            copiedValues[counter2++] = numbersArray[currentSpot + 2];
                        }
                        if (row == 0){
                            val = column;
                        }
                    }
                    if ((startingXY + (width * 3 )) < (height * width * 3)){
                        startingXY = startingXY + (width * 3);
                    }
                    val2 = row;
                }

              
                counter2 = 0;
                unsigned long column;
                unsigned long startingXYPaste = (startRowPaste * width * 3) + (startColumnPaste * 3);
                for(unsigned long row = 0; row <= val2 && ((startRowPaste + row) < height); row++){
                    for(column = 0; column <= val && ((startColumnPaste + column) < width); column++){
                        unsigned long currentSpot = startingXYPaste + (column * 3);

                        if (currentSpot + 2 < width * height * 3){
                            numbersArray[currentSpot] = copiedValues[counter2];
                            numbersArray[currentSpot + 1] = copiedValues[counter2 + 1];
                            numbersArray[currentSpot + 2] = copiedValues[counter2 + 2];
                            counter2 +=3; 
                        }
                    }
                    while (column <= val){ // if column <= val means we cant paste anymore in bc its going to go out of bounds
                        counter2 += 3;  // therefore discard the rest of the values in the row in copiedValues
                        column++;
                    }
                    if ((startingXYPaste + (width * 3)) < (height * width * 3)){
                        startingXYPaste = startingXYPaste + (width * 3);
                    }
                }
              
                for (unsigned long k = 0; k < numCounter; k+=3){
                    fprintf(write, "%lu %lu %lu\n", numbersArray[k], numbersArray[k + 1], numbersArray[k + 2]);
                }
                
            }
            else{
                portion = strtok(lines, " ");
                while (portion != NULL){
                    if (*portion == '*'){ // encounters a *  for example *2 3 10 
                        char *pointer = portion + 1;
                        unsigned long consecutiveOccurrences = strtoul(pointer, NULL, 10); //extract value after the * which is 2

                        portion = strtok(NULL, " "); //move the pointer forwards now we have 3
                        
                        unsigned long colorIndex = strtoul(portion, NULL, 10) * 3; // store this color index 3 which is spot 9 in the uniqueColorArray and take colorIndex+1 and colorIndex+2

                        for (unsigned long k = 0; k < consecutiveOccurrences; k++){ //prints the 3 RGB values stored in the uniqueColorarray based on how the number after the *
                            fprintf(write, "%lu %lu %lu\n", colorsArray[colorIndex], colorsArray[colorIndex + 1], colorsArray[colorIndex + 2]);
                        }

                        portion = strtok(NULL, " "); // move the pointer forwards to 10 now
                    }
                    else {            // encounters a normal number like 8 *2 6 10
                        unsigned long colorIndex = strtoul(portion, NULL, 10) * 3;  // stores the color index 8 which is spot 24 in the uniqueColorArray take next 2 values as well

                        fprintf(write, "%lu %lu %lu\n", colorsArray[colorIndex], colorsArray[colorIndex+1], colorsArray[colorIndex+2]); // prints one time because no preceding * 

                        portion = strtok(NULL, " "); //moves the pointe forwards to *2 so the while loop can reevaluate
                    }
                }
            }
            fprintf(write, "\n");
        
            fclose(write);
            fclose(read);
        }
    }
    
    return 0;
}