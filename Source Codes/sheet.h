#ifndef SHEET_H
#define SHEET_H

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

// Function that identify cells (Name them like A1 , A2 , ... , B1 , ....)
void identify_cells(CELL_INFO * , int , int);

// Display cell's name like a table 
void display_cells(CELL_INFO * , int  , int );

// Function that resize the sheet (keeps the old data if cell exist)
void resize_sheet(CELL_INFO ** , int * , int *);

// Function that shows user All command he can use
void help();

#endif