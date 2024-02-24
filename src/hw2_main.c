#include "hw2.h"

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
    // char* iname;
    // char* oname; 
    // char* cname;
    // char* pname; 
    // char* rname;
    int iflag = 0, oflag = 0, cflag = 0, pflag = 0, rflag = 0;
    int unrecognizedflag = 0;

    while ((opt = getopt(argc, argv, "i:o:c:p:r:")) != -1){
        switch (opt){
            case 'i':
                iflag++;
                if (optarg == NULL){
                    return MISSING_ARGUMENT;
                }
                // iname = optarg;
                break;
            case 'o':
                oflag++;
                if (optarg == NULL){
                    return MISSING_ARGUMENT;
                }
                // oname = optarg;
                break;
            case 'c':
                cflag++;
                if (optarg == NULL){
                    return MISSING_ARGUMENT;
                }
                // cname = optarg;
                break;
            case 'p':
                pflag++;
                if (optarg == NULL){
                    return MISSING_ARGUMENT;
                }
                // pname = optarg;
                break;
            case 'r':
                rflag++;
                if (optarg == NULL){
                    return MISSING_ARGUMENT;
                }
                break;
                // rname = optarg;
            case '?':
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
    return 0;
}
