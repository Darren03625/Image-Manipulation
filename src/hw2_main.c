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
        char *substring = strtok(cname, ",");

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

        char *substring = strtok(pname, ",");

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

    char inputString[strlen(iname)+1];
    strncpy(inputString, iname, strlen(iname));
    inputString[strlen(iname)] = '\0';
    printf("ben");
    
    if (strcmp(filetype1, filetype2) == 0){
        fp = fopen(iname, "w");
        fclose(fp);
    }
    else{
        fp = fopen(iname, "r");  // read from input file
        FILE *file;
        if (strcmp(filetype1, "ppm") == 0 && strcmp(filetype2, "sbu") == 0){ // this means input file is ppm, and output file is sbu, we need to convert input file from ppm to sbu
            inputString[strlen(iname) - 1] = 'u';
            inputString[strlen(iname) - 2] = 'b';
            inputString[strlen(iname) - 3] = 's';
            file = fopen(inputString, "w"); // write to a new output file

            char dimensions[9];
            fgets(dimensions, sizeof(dimensions), fp);  // reads in the PPM, used to skip a line 
            fgets(dimensions, sizeof(dimensions), fp);  // reads in the dimensions and overwrites PPM

            unsigned long width = strtoul(strtok(dimensions, " "), NULL, 10);
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10);

            fgets(dimensions, sizeof(dimensions), fp); // used to skip the line "255" in PPM file

            char lines[100 * width * height + 1];
            unsigned long numbersArray[100 * width * height + 1];
            unsigned long counter = 0;
            while (fgets(lines, sizeof(lines), fp) != NULL){  // gets each line as a string and stores in the array lines
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
            
            fputs("SBU\n", file);  // writes SBU to the file                                             SBU
            fprintf(file, "%ld %ld\n", width, height);  // writes dimensions to the file                 width height
            fprintf(file, "%ld\n", uniqueColorCounter); // writes # unique colors to file                #
            
            for (unsigned long k = 0; k < uniqueColorCounter * 3 - 1; k++){ // writes all the color codes of unique colors
                fprintf(file, "%ld ", uniqueColors[k]);
            }

            fprintf(file, "%ld\n", uniqueColors[uniqueColorCounter * 3 - 1]);

            unsigned long colorEntries[counter]; 
            unsigned long index = 0;
            for(unsigned long k = 0; k < counter; k+=3){  // gets rid of duplicates 

                for (unsigned long j = 0; j < uniqueColorCounter; j++){
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
                        fprintf(file, "*%ld %ld ", counter, colorEntries[k]);
                        counter = 1;
                    }
                    else{
                        fprintf(file, "%ld ", colorEntries[k]);
                    }
        
                }
            }

            fclose(file);
            fclose(fp);

        }
        else{ // this means we need to convert the input file from sbu to output ppm
            inputString[strlen(iname) - 1] = 'm';
            inputString[strlen(iname) - 2] = 'p';
            inputString[strlen(iname) - 3] = 'p';
            file = fopen(inputString, "w");

            char dimensions[9];
            fgets(dimensions, sizeof(dimensions), fp);  // used to skip the SBU header
            fgets(dimensions, sizeof(dimensions), fp);  // used to read dimensions
            
            unsigned long width = strtoul(strtok(dimensions," "), NULL, 10);  // splits the string dimension by " ", extracts width
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10);  // splits the string dimension by " ", extracts height

            fgets(dimensions, sizeof(dimensions), fp); // used to read the # unique colors

            // unsigned long uniqueColorCounter = strtoul(dimensions, NULL, 10); // number of RGB triplets

            char lines[100 * width * height + 1];
            unsigned long colorsArray[100 * width * height + 1]; // stores all the unique colors
            unsigned long counter = 0; // size of colorsArray

            fgets(lines, sizeof(lines), fp); // used to read the unique color values and store in lines
            char *portion = strtok(lines, " "); 
                
            while (portion != NULL){ // parses the unique color values and store it in colorsArray
                colorsArray[counter++] = strtoul(portion, NULL, 10);
                portion = strtok(NULL, " ");
            }

            fgets(lines, sizeof(lines), fp); // used to read the line with color value inputs 

            fputs("PPM\n", file);
            fprintf(file, "%ld %ld\n", width, height);
            fputs("255\n", file);

            // unsigned long colorEntries[20 * width * height + 1];
            // unsigned long numColorEntries = 0;
            portion = strtok(lines, " ");
            while (portion != NULL){
                if (*portion == '*'){ // encounters a *  for example *2 3 10 
                    char *pointer = portion;
                    pointer++;
                    unsigned long consecutiveOccurrences = *pointer  - '0'; //extract value after the * which is 2

                    portion = strtok(NULL, " "); //move the pointer forwards now we have 3
                    
                    unsigned long colorIndex = strtoul(portion, NULL, 10) * 3; // store this color index 3 which is spot 9 in the uniqueColorArray and take colorIndex+1 and colorIndex+2

                    for (unsigned long k = 0; k < consecutiveOccurrences; k++){ //prints the 3 RGB values stored in the uniqueColorarray based on how the number after the *
                        fprintf(file, "%ld %ld %ld ", colorsArray[colorIndex], colorsArray[colorIndex + 1], colorsArray[colorIndex + 2]);
                    }

                    portion = strtok(NULL, " "); // move the pointer forwards to 10 now
                }
                else {            // encounters a normal number like 8 *2 6 10
                    int colorIndex = strtoul(portion, NULL, 10) * 3;  // stores the color index 8 which is spot 24 in the uniqueColorArray take next 2 values as well

                    fprintf(file, "%ld %ld %ld ", colorsArray[colorIndex], colorsArray[colorIndex+1], colorsArray[colorIndex+2]); // prints one time because no preceding * 

                    portion = strtok(NULL, " "); //moves the pointe forwards to *2 so the while loop can reevaluate
                }
                
            }

            fclose(file);
            fclose(fp);

        }
    }   

    return 0;
}
