#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sheet.h"

// Structure and all proto types are defined in HEADER_FILE

void resize_sheet(CELL_INFO **cells , int *rows , int *cols){

    // Copy old rows & cols into r & c
    int r = *rows;
    int c = *cols;

    // Ask for new rows & cols
    printf("\n      Enter New Rows : "); scanf("%d" , rows);
    printf("\n      Enter New Columns : "); scanf("%d" , cols);

    // Clear buffer from "\n" after scanf
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    char x = 'y';
    if (*rows < r || *cols < c) {

        // WARNING!
        printf("\n          Warning : you are reducing rows and cols & they may have some values that are going to miss" );
        printf("\n          Are you sure enter[y/n]?"); scanf("%c" , &x);

        // Clear buffer from "\n" after scanf
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        
        if (x == 'n') {
            *rows = r;
            *cols = c;
            return;
        }
        
        if (x != 'y' && x != 'n'){
            printf("\n          Invalid Input\n" );
            *rows = r;
            *cols = c;
            return;
        }  
    }
    
    // Copy of old sheet (using memcpy)
    CELL_INFO *old_cells = (CELL_INFO *) malloc (r * c * sizeof(CELL_INFO));
    if(!old_cells){
        printf("Backup allocation failed!\n");
        return;
    }
    memcpy(old_cells , *cells , r * c * sizeof(CELL_INFO));

    // Realloc cells by using another pointer to avoid missing data when allocation failure happend
    CELL_INFO *tmp = realloc(*cells, (*rows) * (*cols) * sizeof(CELL_INFO));
    if (!tmp) {
        printf("\nRealloc failed! Keeping old sheet.\n");
        *rows = r;
        *cols = c;
        free(old_cells);
        return;
    }

    //Transforming datas from old sheet to new one
    int index_0;
    int index_1;

    int a = (r < *rows)? r : *rows;
    int b = (c < *cols)? c : *cols;

     // Array List of what cells exists in new sheet from old one
    int arr[a*b];

    int i = 0;
    for (int x = 0 ; x < a ; x++){
        for (int y = 0 ; y < b ; y++){

            // Index of old cells
            index_0 = x * c + y;
            
            // Equivalant index in new cells(temp)
            index_1 = x * (*cols) + y;

            arr[i] = index_1;
            i++;

            tmp[index_1] = old_cells[index_0];
        }
    }

    free(old_cells);
    
    // Update extra cells (fill with 0 & ...) if sheet has been extended
    int index;
    if (*rows > r || *cols > c){
        for(int i = 0 ; i < *rows ; i++){
            for (int j = 0 ; j < *cols ; j++){
                index = i * (*cols) + j;
                int found = 0;
                for(int k = 0 ; k < a * b ; k++)
                    if(arr[k] == index){
                        found = 1;
                        break;
                    }

                if (found == 0){
                    tmp[index].int_num = 0;
                    tmp[index].int_set = 1;
                    tmp[index].float_set = 0;
                    tmp[index].formula_set = 0;
                    tmp[index].status[0] = 1;
                }
            }
        }
    }

    // Sets all changes into Main Cells
    *cells = tmp; 

    identify_cells(*cells, *rows, *cols);    
}

void help(){

    // List of commands
    printf("\n      list of operators you can use : \n");
    printf("\n      command 1 - resize sheet (or rs as short command)");
    printf("\n                  by using this command you can increase or reduce num of rows and columns");
    printf("\n                  BECAREFUL : reducing rows or cols may delete your datas from sheet\n");
    printf("\n      command 2 - show cells (or sc as short command)");
    printf("\n                  by using this command you can see current sheet pattern\n");

}

void identify_cells(CELL_INFO *cells , int rows , int cols){

    // 0 <= r <= rows - 1
    // 0 <= c <= cols - 1
    // index formula : r * cols + c

    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            cells[index].name[0] = (char) ((int)('A') + c);
            cells[index].name[1] = (char) (r + 1 + 48); 
            cells[index].name[2] = '\0';            
        }
    }
}

void display_cells(CELL_INFO *cells , int rows , int cols){

    // 0 <= r <= rows - 1
    // 0 <= c <= cols - 1
    // index formula : r * cols + c
        
    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            printf("%s    " , cells[index].name);
        }
        printf("\n");
    }
}