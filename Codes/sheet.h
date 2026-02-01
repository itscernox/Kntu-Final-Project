#ifndef SHEET_H
#define SHEET_H
 
#include "common.h"

// Function that identify cells (Name them like A1 , A2 , ... , B1 , ....)
void identify_cells(CELL_INFO * , int , int);

// Display cell's name like a table 
void display_cells(CELL_INFO * , int  , int );

// Function that resize the sheet (keeps the old data if cell exist)
void resize_sheet(CELL_INFO ** , int * , int *);

// Function to set numberic values by analyzing string
void set_value(CELL_INFO ** , int , int);

// Function to report cell value & value type  
void cell_info(CELL_INFO * , int  , int );

// Function to delay 1 second for proccessing lines
void delay_seconds(int seconds);

// Function that shows user All command he can use
void help();

#endif