#include "hw2.h"

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
        for (int i = 0; cname[i] != '\0'; i++){
            if (strtoul(&cname[i], NULL, 10) > 0 && strtoul(&cname[i+1], NULL, 10) == 0){
                counter++;
            }
        }
        if(counter != 4){
        return C_ARGUMENT_INVALID;
        }
    }

    if (pname != NULL){
        int counter = 0;
        for (int i = 0; pname[i] != '\0'; i++){
            if (strtoul(&pname[i], NULL, 10) > 0 && strtoul(&pname[i+1], NULL, 10) == 0){
                counter++;
            }
        }
        if (counter != 2){
        return P_ARGUMENT_INVALID;
        }
    }

    if (rname != NULL){
        int counter = 1;
        for (int i = 0; rname[i] != '\0'; i++){
            if (rname[i] == ',' && rname[i] != '\0'){
                counter++;
            }
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
            fgets(dimensions, sizeof(dimensions), fp);
            fgets(dimensions, sizeof(dimensions), fp);

            unsigned long width = strtoul(strtok(dimensions," "), NULL, 10);
            unsigned long height = strtoul(strtok(NULL, " "), NULL, 10);

            fgets(dimensions, sizeof(dimensions), fp);

            char lines[20 * width * height + 1];
            unsigned long numbersArray[20 * width * height + 1];
            unsigned long counter = 0;
            while (fgets(lines, sizeof(lines), fp) != NULL){
                
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
            unsigned long uniqueColorCounter = 0;
            for (i = 0; i < counter; i+= 3){

                int unique = 1;
                for (unsigned long j = 0; j < uniqueColorCounter; j++){
                    if (uniqueColors[j * 3] == numbersArray[i] && uniqueColors[j * 3 + 1] == numbersArray[i+1] && uniqueColors[j * 3 + 2] == numbersArray[i+2]){
                        unique = 0;
                        break;
                    } 
                }

                if (unique == 1){
                    uniqueColors[uniqueColorCounter * 3] = numbersArray[i];
                    uniqueColors[uniqueColorCounter * 3 + 1] = numbersArray[i + 1];
                    uniqueColors[uniqueColorCounter * 3 + 2] = numbersArray[i+ 2];
                    uniqueColorCounter++;
                }
            }
            
            fputs("SBU\n", file);
            fprintf(file, "%ld %ld\n", width, height);
            fprintf(file, "%ld ", uniqueColorCounter);
            
            for (i = 0; i < uniqueColorCounter * 3 - 1; i++){
                fprintf(file, "%ld ", uniqueColors[i]);
            }

            fprintf(file, "%ld\n", uniqueColors[uniqueColorCounter * 3 - 1]);

            unsigned long colorEntries[counter]; 
            unsigned long index = 0;
            for(i = 0; i < counter; i+=3){

                for (unsigned long j = 0; j < uniqueColorCounter; j++){
                    if (uniqueColors[j * 3] == numbersArray[i] && uniqueColors[j * 3 + 1] == numbersArray[i+1] && uniqueColors[j * 3 + 2] == numbersArray[i+2]){
                        colorEntries[index++] = j; 
                        break;
                    }
                }
            }

            
            counter = 1;
            for (i = 0; i < index; i++){
                if (i < index - 1 && colorEntries[i] == colorEntries[i+1]){
                    counter++;
                }
                else{
                    if (counter > 1){
                        fprintf(file, "*%ld %ld ", counter, colorEntries[i]);
                        counter = 1;
                    }
                    else{
                        fprintf(file, "%ld ", colorEntries[i]);
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
            fputs("PPM\n", file);

            fclose(file);
            fclose(fp);


        }
    }

    return 0;
}
