#ifndef COMMON_H
#define COMMON_H

// All Used Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "sheet.h"
#include "fileio.h"

// Structure that save cells informatin ( Name , value , value type , formula , .....)
typedef struct{
    char name[3];
    long long int int_num;
    int int_set;
    double float_num;
    int float_set;   
    char formula[100];
    int formula_set;    

    /*
        status show what kind of data does the sell have stored
        1 : int 
        2 : float 
    */
   
    int status[1];
} CELL_INFO;

#endif