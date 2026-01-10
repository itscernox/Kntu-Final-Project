#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sheet.h"

// Check operator to call appopriate function
void check_operator(char [] , int *  , int * , CELL_INFO **);

int main(){

    // Operator command string
    char opt[50];

    // Default Rows and Columns at begining
    int rows = 5 , cols = 5;

    // Main sheet cells structure
    CELL_INFO *cells;
    cells = (CELL_INFO *) malloc (rows * cols * sizeof(CELL_INFO));
    if (!cells) { 
        printf("Allocation Failed!\n"); 
        return 1; 
    }

    // Notes for better User Interface
    printf("\nHi , Welcome to the  SESS (Simple Excell Spreed Sheet)\n");
    printf("\nBy enterting opt (help) or ( hp (short form) ) you can see the list of operations\n");
    printf("\nBY entering opt  (EXIT) or ( ext (short form) ) you will leave the program\n");
    printf("\nDefault chart is 5x5\n\n");

    // Fuinction from sheet.h
    identify_cells(cells , rows , cols);

    // Fuinction from sheet.h
    display_cells(cells , rows , cols);

    // Update cells info at begining (Fill cells with 0 at start & ....)
    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            cells[index].int_num = 0;
            cells[index].int_set = 1;
            cells[index].float_set = 0;
            cells[index].formula_set = 0;
            cells[index].status[0] = 1;
        }
    }

    printf("\n");
    
    printf("---------------------------------------------------------------------------------------------");

    printf("\n\n");

    // Get opt from user until command "exit"
    do{
        printf("Input Operator  >> ");
        fgets(opt , sizeof(opt) , stdin);
        opt[strcspn(opt , "\n")] = '\0';

        // Call check_operator if opt is not exit
        if (strcmp(opt , "exit") !=0  && strcmp(opt , "ext") != 0)
            check_operator(opt , &rows , &cols , &cells);

        printf("\n---------------------------------------------------------------------------------------------\n");
        printf("\n");
    } while(strcmp(opt,"exit") != 0 && strcmp(opt , "ext") != 0);
    
    free(cells);
    return 0;
}

void check_operator(char opt[] , int * rows , int * cols , CELL_INFO **cells){
    
    if (strcmp(opt , "help") == 0 || (strcmp(opt , "hp") == 0)){
        help();
    }

    else if (strcmp(opt , "resize sheet") == 0 || strcmp(opt , "rs") == 0){
        resize_sheet(cells , rows , cols);
    }

    else if (strcmp(opt , "show cells") == 0 || strcmp(opt , "sc") == 0){
        printf("\n");
        display_cells(*cells , *rows , *cols);
    }

    else
        printf("\n      Your Input is not valid!\n");
}