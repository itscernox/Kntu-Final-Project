#ifndef FILEIO_H
#define FILEIO_H

#include "common.h"


// Function for intialization part (Build new spreed sheet)
int new_sheet(CELL_INFO ** , int * , int * , FILE **);

// Funtion for intialization part (Import spreed sheet)
int load_binary(char * , CELL_INFO ** , int * , int * , FILE **);

// Function to Commit all changes to disk () file)
void save(FILE * , CELL_INFO * , int , int );

// Build sheet report txt file after exit
void export_txt(CELL_INFO * , int , int);

#endif