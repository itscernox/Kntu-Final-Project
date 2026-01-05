#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct  CELL_INFO{
    char name[3];
    long long int int_num;
    int int_set;
    double float_num;
    int float_set;

    /*
        status show what kind of data does the sell have stored
        0 : None
        1 : int 
        2 : float 
        3 : double 
    */
   
    int status[1];
};

void identify_cells(struct CELL_INFO * , int , int);
void display_cells(struct CELL_INFO * , int  , int );
void check_operator(char [] , int *  , int * , struct CELL_INFO **);
void help();
void sheet_settings(struct CELL_INFO ** , int * , int *);
//void Data_Transformation(struct CELL_INFO * , int , int , int , int);


int main(){

    char opt[50];
    int rows = 5 , cols = 5;
    struct CELL_INFO *cells;

    cells = (struct CELL_INFO *) malloc (rows * cols * sizeof(struct CELL_INFO));
    if (!cells) { 
        printf("Allocation Failed\n"); 
        return 1; 
    }

    printf("\nHi , Welcome to the  SESS (Simple Excell Spreed Sheet)\n");
    printf("\nBy enterting opt (help) or ( hp (short form) ) you can see the list of operations\n");
    printf("\nBY entering opt  (EXIT) or ( ext (short form) ) you will leave the program\n");
    printf("\nDefault chart is 5x5\n\n");


    identify_cells(cells , rows , cols);

    display_cells(cells , rows , cols);

    // set cells status to not filled at start
    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            cells[index].int_set = 0;
            cells[index].float_set = 0;
            cells[index].status[0] = 0;
        }
    }

    printf("\n");
    
    printf("---------------------------------------------------------------------------------------------");

    printf("\n\n");

    do{
        printf("Input Operator  >> ");
        fgets(opt , sizeof(opt) , stdin);
        opt[strcspn(opt , "\n")] = '\0';

        if (strcmp(opt , "exit") !=0  && strcmp(opt , "ext") != 0)
            check_operator(opt , &rows , &cols , &cells);

        printf("\n---------------------------------------------------------------------------------------------\n");
        printf("\n");
    } while(strcmp(opt,"exit") != 0 && strcmp(opt , "ext") != 0);
    
    free(cells);
    return 0;
}

void check_operator(char opt[] , int * rows , int * cols , struct CELL_INFO **cells){
    if (strcmp(opt , "help") == 0 || (strcmp(opt , "hp") == 0)){
        help();
    }

    else if (strcmp(opt , "sheet settings") == 0 || strcmp(opt , "ss") == 0){
        sheet_settings(cells , rows , cols);
    }

    else if (strcmp(opt , "show cells") == 0 || strcmp(opt , "sc") == 0){
        printf("\n");
        display_cells(*cells , *rows , *cols);
    }

    else
        printf("\n      Your Input is not valid!\n");
}

void sheet_settings(struct CELL_INFO **cells , int *rows , int *cols){
    int r = *rows;
    int c = *cols;
    printf("\n      Enter New Rows : "); scanf("%d" , rows);
    printf("\n      Enter New Columns : "); scanf("%d" , cols);

    int ch;
    //Clear buffer from newline
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    char x = 'y';
    if (*rows < r || *cols < c) {
        printf("\n          Warning : you are reducing rows and cols & they may have some values that are going to miss" );
        printf("\n          Are you sure enter[y/n]?"); scanf("%c" , &x);
        //Clear buffer from newline
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (x == 'n') {
            *rows = r;
            *cols = c;
            return;
        } 
    }

    struct CELL_INFO *tmp = realloc(*cells, (*rows) * (*cols) * sizeof(struct CELL_INFO));

    if (!tmp) {
        printf("\nRealloc failed! Keeping old sheet.\n");
        *rows = r;
        *cols = c;
        return;
    }

    *cells = tmp;  

    identify_cells(*cells, *rows, *cols);    
}

// void Data_Transformation()

void help(){
    printf("\n      list of operators you can use : \n");
    printf("\n      1 - sheet settings (or ss as short command)");
    printf("\n          by using this command you can increase or reduce num of rows and columns");
    printf("\n          BECAREFUL : reducing rows or cols may delete your datas from sheet\n");
    printf("\n      2 - show cells (or sc as short command)");
    printf("\n          by using this command you can see current sheet setting\n");


}

void identify_cells(struct CELL_INFO *cells , int rows , int cols){

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

void display_cells(struct CELL_INFO *cells , int rows , int cols){
    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            printf("%s    " , cells[index].name);
        }
        printf("\n");
    }
}