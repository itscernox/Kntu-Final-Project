//Next steps : set value & set formula function || cell info func (print value & type of vlaue & formula condition & ...)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct  CELL_INFO{
    char name[3];
    long long int int_num;
    int int_set;
    double float_num;
    int float_set;   
    char formula[100];
    int formula_set;    

    /*
        status show what kind of data does the sell have stored
        0 : None
        1 : int 
        2 : float 
    */
   
    int status[1];
};

void identify_cells(struct CELL_INFO * , int , int);
void display_cells(struct CELL_INFO * , int  , int );
void check_operator(char [] , int *  , int * , struct CELL_INFO **);
void resize_sheet(struct CELL_INFO ** , int * , int *);
void help();

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

    //Fill cells with 0 at start & update cells info
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

void resize_sheet(struct CELL_INFO **cells , int *rows , int *cols){
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
    
    //make a copy of old sheet
    //struct CELL_INFO *old_cells = *cells;
    struct CELL_INFO *old_cells = (struct CELL_INFO *) malloc (r * c * sizeof(struct CELL_INFO));
    if(!old_cells){
        printf("Backup allocation failed!\n");
        return;
    }
    memcpy(old_cells , *cells , r * c * sizeof(struct CELL_INFO));

    //realloc cells in another pointer to avoid missing data when allocation failure happend
    struct CELL_INFO *tmp = realloc(*cells, (*rows) * (*cols) * sizeof(struct CELL_INFO));

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
    int arr[a*b];
    int i = 0;
    for (int x = 0 ; x < a ; x++){
        for (int y = 0 ; y < b ; y++){
            // index of old cells
            index_0 = x * c + y;
            
            // equivalant index in new cells(temp)
            index_1 = x * (*cols) + y;

            arr[i] = index_1;
            i++;

            /*
            if (old_cells[index_0].int_set == 1){
                tmp[index_1].int_num = old_cells[index_0].int_num;
            }

            else if (old_cells[index_0].float_set == 1){
                tmp[index_1].float_num = old_cells[index_0].float_num;
            }

            if (old_cells[index_0].formula_set == 1){
                strncpy(tmp[index_1].formula, old_cells[index_0].formula, sizeof(tmp[index_1].formula));
                tmp[index_1].formula[sizeof(tmp[index_1].formula) - 1] = '\0';
            }

            tmp[index_1].int_set = old_cells[index_0].int_set;
            tmp[index_1].float_set = old_cells[index_0].float_set;
            tmp[index_1].formula_set = old_cells[index_0].formula_set;
            tmp[index_1].status[0] = old_cells[index_0].status[0];
            */


            // the line below do same as the upper commented code because struct contains only fixed-size arrays
            // copy the entire cell
            tmp[index_1] = old_cells[index_0];
        }
    }

    free(old_cells);
    
    //update extra cells(fill with 0 & ...) if sheet has been extended
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

    *cells = tmp;  
    identify_cells(*cells, *rows, *cols);    
}

void help(){
    printf("\n      list of operators you can use : \n");
    printf("\n      command 1 - resize sheet (or rs as short command)");
    printf("\n                  by using this command you can increase or reduce num of rows and columns");
    printf("\n                  BECAREFUL : reducing rows or cols may delete your datas from sheet\n");
    printf("\n      command 2 - show cells (or sc as short command)");
    printf("\n                  by using this command you can see current sheet pattern\n");


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