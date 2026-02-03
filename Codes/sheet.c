#include "common.h"

void delay_seconds(int seconds){
    time_t start = time(NULL);
    while (time(NULL) - start < seconds);
}

void resize_sheet(CELL_INFO **cells , int *rows , int *cols){

    // Warning! (limit of sheet setting)
     printf("\n      WARNING : There is a limit set for rows and columns. Maximum 26 columns and 9 rows.\n");

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
        printf("\n      Warning : you are reducing rows and cols & they may have some values that are going to miss" );
        printf("\n      Are you sure enter[y/n]? "); scanf("%c" , &x);

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
    
    delay_seconds(1);
    printf("\n      Done"); 
}

void set_value(CELL_INFO **cells , int rows , int cols){

    // Input Cell's Name
    char str[3];
    printf("\n      Enter Cells Name : "); scanf("%2s" , str);

    // Clear buffer from "\n" after scanf
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    int index;
    int found = 0;
    for (int r = 0 ; r < rows ; r++)
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            if (strcmp((*cells)[index].name , str) == 0){
                found = 1;
                char input[100];
                printf("      Enter Number to SET : ");
                fgets(input , sizeof(input) , stdin);
                input[strcspn(input , "\n")] = '\0';
                char *endptr;
                errno = 0;
                (*cells)[index].int_num = strtol(input , &endptr , 10);
                if (errno == 0 && *endptr == '\0'){
                    printf("      Confirm to set (%lld) as value by entering [y/n] : " , (*cells)[index].int_num);
                    char x = 'y';
                    scanf("%c" , &x);

                    // Clear buffer from "\n" after scanf
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}

                    if (x == 'n'){
                        (*cells)[index].int_num = 0;
                    }

                    if (x != 'y' && x != 'n'){
                        printf("      invalid input!\n");
                        return;
                    }
                    
                    printf("      Value Set successfully\n");
                    (*cells)[index].int_set = 1;
                    (*cells)[index].float_set = 0;
                    (*cells)[index].status[0] = 1;
                    return;
                }
                
                errno = 0;
                (*cells)[index].float_num = strtof(input , &endptr);
                if (errno == 0 && *endptr == '\0'){
                    printf("          Confirm to set (%lf) as value by entering [y/n] : " , (*cells)[index].float_num);
                    char x = 'y';
                    scanf("%c" , &x);
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    if (x == 'n'){
                        (*cells)[index].float_num = 0.0;
                    }
                    if (x != 'y' && x != 'n'){
                        printf("      invalid input!\n");
                        return;
                    }
                    printf("      Value Set successfully\n");
                    (*cells)[index].int_set = 0;
                    (*cells)[index].float_set = 1;
                    (*cells)[index].status[0] = 2;
                    return;
                }

                printf("      error : please enter a numberic input\n");
                return;
            }
        }
    
    if (found == 0){
        printf("\n      Cell Not Found!");
        return;
    }

    // Recalculate the value of cells which has set with formula
    recalculate_all(*cells , rows , cols);

}

void cell_info(CELL_INFO *cells , int rows , int cols){
    char str[3];
    printf("\n      Enter Cells Name : "); scanf("%s" , str);

    // Clear buffer from "\n" after scanf
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    int index;
    int found = 0;
    for (int r = 0 ; r < rows ; r++)
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            if (strcmp(cells[index].name , str) == 0){
                found = 1;
                printf("\n");
                if (cells[index].status[0] == 1){
                    printf("      Cell Name : %s\n" , cells[index].name);
                    printf("      Data Type : integer\n");
                    printf("      Value : %lld\n" , cells[index].int_num);
                }
                else if (cells[index].status[0] == 2){
                    printf("      Cell Name : %s\n" , cells[index].name);
                    printf("      Data Type : float\n");
                    printf("      Value : %lf\n" , cells[index].float_num);
                }
                return;
            }
        }

    if (found == 0){
        printf("\n      Cell Not Found!");
        return;
    }
}

void help(){
    printf("\n      list of operators you can use : \n");
    printf("\n      command 1 - resize sheet (or rs as short command)");
    printf("\n                  by using this command you can increase or reduce num of rows and columns");
    printf("\n                  BECAREFUL : reducing rows or cols may delete your datas from sheet\n");
    printf("\n      command 2 - show cells (or sc as short command)");
    printf("\n                  by using this command you can see current sheet pattern\n");
    printf("\n      command 3 - set value (or sv as short command)");
    printf("\n                  by using this command you can set numberic values to cells\n");
    printf("\n      command 4 - cell info (or cf as short command)");
    printf("\n                  by using this command you can see info like (value & datatypes) about cell \n");
    printf("\n      command 5 - set formula (or sf as short command)");
    printf("\n                  by using this command you can set formula for the cell you enter , then it will calculate the result\n");
    printf("\n                  and result will be saved as the new value of cell\n");
    printf("\n      command 6 - save");
    printf("\n                  by using this command you can commit and save your changes into disk\n");
    printf("\n      command 7 - exit");
    printf("\n                  by using this command you can terminate the programm and quit\n");
    
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