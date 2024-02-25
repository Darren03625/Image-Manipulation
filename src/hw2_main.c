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
    char* iname, *oname, *cname, *pname, *rname;
    int iflag = 0, oflag = 0, cflag = 0, pflag = 0, rflag = 0;
    int unrecognizedflag = 0;

    while ((opt = getopt(argc, argv, "i:o:c:p:r:")) != -1){
        printf("OPT: %c\n", opt);
        fflush(stdout);
        switch (opt){
            case 'i':
                iflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                iname = optarg;
                printf("%s\n", iname);
                break;
            case 'o':
                oflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                oname = optarg;
                printf("%s\n", oname);
                break;
            case 'c':
                cflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                cname = optarg;
                printf("%s\n", cname);
                break;
            case 'p':
                pflag++;
                if (optind < argc){
                    if (optarg[0] == '-'){
                        return MISSING_ARGUMENT;
                    }
                }
                pname = optarg;
                printf("%s\n", pname);
                break;
            case 'r':
                rflag++;
                if (optarg[0] == '-'){
                    return MISSING_ARGUMENT;
                }
                rname = optarg;
                printf("%s\n", rname);
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
    FILE *fp = fopen(iname, "r");
    if (fp == NULL){
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

    // if (pname != NULL && cname == NULL){
    //     return C_ARGUMENT_MISSING;
    // }
    return 0;
}
