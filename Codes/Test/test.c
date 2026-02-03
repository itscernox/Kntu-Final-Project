// Next steps : 
// 1 - set value & set formula function || cell info func (print value & type of vlaue ) : (Done)
// 2 - build file.c (I think i have a idea) : (Done) 
// 3 - formula.c (Done)  (in this part i mostly used educational youtube resources & Google Gemini AI)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <ctype.h>


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
        1 : int 
        2 : float 
    */

    int status[1];
};

void check_operator(char [] , int *  , int * , struct CELL_INFO ** , FILE * , int *);
void identify_cells(struct CELL_INFO * , int , int);
void display_cells(struct CELL_INFO * , int  , int );
void resize_sheet(struct CELL_INFO ** , int * , int *);
void set_value(struct CELL_INFO ** , int , int);
void cell_info(struct CELL_INFO * , int  , int );
void delay_seconds(int);
int new_sheet(struct CELL_INFO ** , int * , int * , FILE **);
int load_binary(char * , struct CELL_INFO ** , int * , int * , FILE **);
void save(FILE * , struct CELL_INFO * , int , int );
void export_txt(struct CELL_INFO * , int , int);

// ---------------------- < Formula Engine > ----------------------

// Global variables needed for the parser parsing state
const char *expression_ptr;
struct CELL_INFO *sheet_cells;
int sheet_rows;
int sheet_cols;
int parse_error; // 0: OK, 1: Error

void recalculate_all(struct CELL_INFO *, int , int);
double evaluate_formula(char *, struct CELL_INFO *, int , int , int *);
double get_cell_value_by_name(char *);
double parse_expression();
double parse_term();
double parse_factor();
void skip_whitespace();

//------------------------------------------------------------------
void help();

int main(){

    printf("\n------------------------------------------------------------\n");
    printf("   SESS - Simple Excel Spreadsheet Simulator\n");
    printf("------------------------------------------------------------\n\n");
    printf("This program is a console-based spreadsheet simulator\n");
    printf("Designed to demonstrate fundamental concepts of data\n");

    printf("\nInitialization Options:\n");
    printf("  1. Load existing spreadsheet from binary file\n");
    printf("  2. Create a new spreadsheet\n\n");
    printf("Please select an option [1/2]");
    printf("\n\n");

    int choice;
    do{
        printf("Type here >> ");
        scanf("%d" , &choice);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (choice != 1 && choice != 2)
            printf("Invalid Input!\n\n");
        printf("\n");
    } while(choice != 1 && choice != 2);

    FILE *f = NULL;
    struct CELL_INFO *cells;
    int rows , cols;

    if (choice == 1){
        char filename[20];
        printf("Enter Database File Name : ");
        scanf("%s" , filename);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (!(load_binary(filename , &cells , &rows , &cols , &f))){
            printf("\nFile Not Found!\n\n");
            printf("Terminating Program....\n");
            delay_seconds(1);
            return 1;
        }
    }

    else{
        if(!(new_sheet(&cells , &rows , &cols , &f))){
            return 1;
        }
    }

    printf("Everything is ready.\n\n");
    printf("Type 'help' or 'hp' to view the list of available commands.\n");
    printf("Type 'save' to commit changes to disk.\n");
    printf("Type 'exit' to terminate the program.\n");
    printf("------------------------------------------------------------\n\n");

    display_cells(cells , rows , cols);

    printf("\n");
    
    printf("---------------------------------------------------------------------------------------------");

    printf("\n\n");

    char opt[50];
    int is_save;
    do{ 
        printf("Input Operator  >> ");
        fgets(opt , sizeof(opt) , stdin);
        opt[strcspn(opt , "\n")] = '\0';

        if (strcmp(opt , "exit") !=0  && strcmp(opt , "ext") != 0)
            check_operator(opt , &rows , &cols , &cells , f , &is_save);
        
        if (strcmp(opt , "exit") == 0 || strcmp(opt , "ext") == 0)
            if (is_save == 0){
                printf("\n      Spreed Sheet is not saved!! Do you want to save changes [y/n]? ");
                char x = 'y';
                do{
                    scanf("%c" , &x);
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}

                    if (x == 'y'){
                        errno = 0;
                        save(f , cells , rows , cols);
                        if (errno == 0){
                            printf("\n      Saved! Binary file updated succesfully");
                            is_save = 1;
                        }
                        else{
                            printf("\n      Error! Something went wrong");
                            is_save = 0;
                        }
                    }

                    else if (x == 'n'){
                        is_save = 0;
                    }
                } while (x != 'n' && x != 'y');
            }

        printf("\n---------------------------------------------------------------------------------------------\n");
        printf("\n");
    } while(strcmp(opt,"exit") != 0 && strcmp(opt , "ext") != 0);

    fclose(f);

    //build txt file;
    delay_seconds(1);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    export_txt(cells , rows , cols);
    printf("Current Sheet report saved in %04d-%02d-%02d %02d:%02d:%02d.txt\n\n",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);

    free(cells);

    delay_seconds(1);
    printf("Terminating the program....");
    delay_seconds(1);
    return 0;
}

void delay_seconds(int seconds){
    time_t start = time(NULL);
    while (time(NULL) - start < seconds);
}

int new_sheet(struct CELL_INFO **cells , int *rows , int *cols , FILE **f){
    printf("Creating a new spreadsheet...\n\n");
    delay_seconds(1);
    printf("Default sheet size is set to 5 rows x 5 columns.\n\n");

    *rows = 5; 
    *cols = 5;

    *cells = (struct CELL_INFO *) malloc ((*rows) * (*cols) * sizeof(struct CELL_INFO));
    if (!(*cells)){ 
        printf("Error : Memory Allocation Failed!\n"); 
        return 0; 
    }

    identify_cells(*cells , *rows , *cols);

    *f = fopen("database" , "wb");
    fwrite(&rows , sizeof(int) , 1 , *f);
    fwrite(&cols , sizeof(int) , 1 , *f);

    //Fill cells with 0 at start & update cells info
    int index;
    for (int r = 0 ; r < *rows ; r++){
        for (int c = 0 ; c < *cols ; c++){
            index = r * (*cols) + c;
            (*cells)[index].int_num = 0;
            (*cells)[index].int_set = 1;
            (*cells)[index].float_set = 0;
            (*cells)[index].formula_set = 0;
            (*cells)[index].status[0] = 1;
            fwrite(&(*cells)[index] , sizeof(struct CELL_INFO) , 1 ,*f);
        }
    }

    fclose(*f);
    *f = fopen("database" , "rb+");

    delay_seconds(1);
    printf("New spreed sheet created. Binary File Name : database\n\n");
    delay_seconds(1);

    return 1;
}

int load_binary(char *filename , struct CELL_INFO **cells , int *rows , int *cols , FILE **f){
    *f = fopen(filename , "rb");
    //*f = fopen("/Users/cernox/Desktop/db" , "rb");

    delay_seconds(1);
    printf("\nLoading spreadsheet...\n");
    delay_seconds(1);

    if (!(*f))
        return 0;

    fread(rows , sizeof(int) , 1 , *f);
    fread(cols , sizeof(int) , 1 , *f);

    *cells = (struct CELL_INFO *) malloc ((*rows) * (*cols) * sizeof(struct CELL_INFO));
    if (!(*cells)) { 
        printf("Error : Memory Allocation Failed!\n"); 
        return 0; 
    }

    int index;
    for(int r = 0 ; r < *rows ; r++)
        for (int c = 0 ; c < *cols ; c++){
            index = r * (*cols) + c;
            fread(&(*cells)[index] , sizeof(struct CELL_INFO) , 1 , *f);
        }

    fclose(*f);

    *f = fopen("database" , "rb+");
    //*f = fopen("/Users/cernox/Desktop/db" , "rb+");
    
    printf("\nSpreed Sheet Imported Successfully\n\n");
    delay_seconds(1);

    return 1;
}

void save(FILE *f , struct CELL_INFO * cells, int rows , int cols){
    fseek(f , 0 , SEEK_SET);
    fwrite(&rows , sizeof(int) , 1 , f);
    fwrite(&cols , sizeof(int) , 1 , f);
    int index;
    for (int r = 0 ; r < rows ; r++)
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            fwrite(&(cells[index]) , sizeof(struct CELL_INFO) , 1 , f);
        }
}

void export_txt(struct CELL_INFO *cells , int rows , int cols){
    char filename[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    sprintf(filename,
            "Sheet Report_%04d-%02d-%02d_%02d-%02d-%02d.txt",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);

    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Could not create txt export file\n");
        return;
    }

    fprintf(f, "Spreadsheet Export\n");
    fprintf(f, "Date: %04d-%02d-%02d %02d:%02d:%02d\n\n",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);

    int index;
    for (int r = 0 ; r < rows ; r++){
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            if (cells[index].status[0] == 1)
                fprintf(f , "CELL  %s | INT | %lld" , cells[index].name , cells[index].int_num); 
            else if(cells[index].status[0] == 2)
                fprintf(f , "CELL  %s | FLOAT | %lf" , cells[index].name , cells[index].float_num);
            fprintf(f , "\n------------------------------------------------------------\n");
        }
    }
    fclose(f);
}

void check_operator(char opt[] , int * rows , int * cols , struct CELL_INFO **cells , FILE *f , int *is_save){
    if (strcmp(opt , "help") == 0 || (strcmp(opt , "hp") == 0)){
        help();
    }

    else if (strcmp(opt , "resize sheet") == 0 || strcmp(opt , "rs") == 0){
        resize_sheet(cells , rows , cols);
        *is_save = 0;
    }

    else if (strcmp(opt , "show cells") == 0 || strcmp(opt , "sc") == 0){
        printf("\n");
        display_cells(*cells , *rows , *cols);
    }

    else if (strcmp(opt , "set value") == 0 || strcmp(opt , "sv") == 0){
        set_value(cells , *rows , *cols);
        *is_save = 0;
    }

    else if(strcmp(opt , "cell info") == 0 || strcmp(opt , "cf") == 0){
        cell_info(*cells , *rows , *cols);
    }

    else if(strcmp(opt , "save") == 0 || strcmp(opt , "s") == 0){
        errno = 0;
        save(f , *cells , *rows , *cols);
        if (errno == 0){
            printf("\n      Saved! Binary file updated succesfully");
            *is_save = 1;
        }
        else{
            printf("\n      Error! Something went wrong");
            *is_save = 0;
        }
    }

    else if (strcmp(opt , "set formula") == 0 || strcmp(opt , "sf") == 0){

        char str[3];
        printf("\n      Enter Cells Name : "); scanf("%2s" , str);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        int index;
        int found = 0;
        for (int r = 0 ; r < *rows ; r++)
            for (int c = 0 ; c < *cols ; c++){
                index = r * *cols + c;
                if (strcmp((*cells)[index].name , str) == 0){
                    found = 1;
                    char input_formula[100];
                    printf("      Enter Formula: ");
                    fgets(input_formula, sizeof(input_formula), stdin);
                    input_formula[strcspn(input_formula, "\n")] = '\0';
                    strcpy((*cells)[index].formula, input_formula);
                    (*cells)[index].formula_set = 1;

                    errno = 0;
                    double res = evaluate_formula(input_formula, *cells, *rows, *cols, &errno);
                    
                    if (errno == 0) {
                        (*cells)[index].float_num = res;
                        (*cells)[index].float_set = 1;
                        (*cells)[index].int_set = 0;
                        (*cells)[index].status[0] = 2; // Treat result as float
                        printf("      Formula set! Result calculated: %lf\n", res);
                    } 
                    
                    else {
                        printf("      Formula Error! Value not updated.\n");
                    }

                    *is_save = 0;
                    break;
                }
            }

        if (found == 0){
            printf("\n      Cell Not Found!");
        }
    }

    else
        printf("\n      Your Input is not valid!\n");
}

void resize_sheet(struct CELL_INFO **cells , int *rows , int *cols){
    printf("\n      WARNING : There is a limit set for rows and columns. Maximum 26 columns and 9 rows.\n");
    int r = *rows;
    int c = *cols;
    printf("\n      Enter New Rows : "); scanf("%d" , rows);
    printf("\n      Enter New Columns : "); scanf("%d" , cols);

    int ch;
    //Clear buffer from newline
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    if (*rows > 9 || *cols > 26){
        printf("\n     Failed! outside the specified limit.");
        *rows = r;
        *cols = c;
        return;
    }

    char x = 'y';
    if (*rows < r || *cols < c) {
        printf("\n      Warning : you are reducing rows and cols & they may have some values that are going to miss" );
        printf("\n      Are you sure enter[y/n]? "); scanf("%c" , &x);
        //Clear buffer from newline
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
    
    //make a copy of old sheet
    //struct CELL_INFO *old_cells = *cells;
    struct CELL_INFO *old_cells = (struct CELL_INFO *) malloc (r * c * sizeof(struct CELL_INFO));
    if(!old_cells){
        printf("      Backup allocation failed!\n");
        return;
    }
    memcpy(old_cells , *cells , r * c * sizeof(struct CELL_INFO));

    //realloc cells in another pointer to avoid missing data when allocation failure happend
    struct CELL_INFO *tmp = realloc(*cells, (*rows) * (*cols) * sizeof(struct CELL_INFO));
  
    if (!tmp) {
        printf("\n      Realloc failed! Keeping old sheet.\n");
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
    delay_seconds(1);
    printf("\n      Done");     
}

void set_value(struct CELL_INFO **cells , int rows , int cols){

    char str[3];
    printf("\n      Enter Cells Name : "); scanf("%2s" , str);

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
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    if (x == 'n'){
                        (*cells)[index].int_num = 0;
                    }
                    if (x != 'y' && x != 'y'){
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

    recalculate_all(*cells , rows , cols);
}

void cell_info(struct CELL_INFO *cells , int rows , int cols){
    char str[3];
    printf("\n      Enter Cells Name : "); scanf("%s" , str);
    int ch;
    //Clear buffer from newline
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

double evaluate_formula(char *formula_str, struct CELL_INFO *cells, int r, int c, int *error) {

    expression_ptr = formula_str;
    sheet_cells = cells;
    sheet_rows = r;
    sheet_cols = c;
    parse_error = 0;
    
    // Check for empty formula
    if (formula_str == NULL || strlen(formula_str) == 0) 
        return 0.0;
    
    // Skip '=' if present (e.g. "=A1+B2")
    if (*expression_ptr == '=') 
        expression_ptr++;
    
    double result = parse_expression();
    
    if (parse_error) 
        *error = 1;
    else 
        *error = 0;
    
    return result;
}

double get_cell_value_by_name(char *name) {
    int index;
    for (int r = 0; r < sheet_rows; r++) {
        for (int c = 0; c < sheet_cols; c++) {
            index = r * sheet_cols + c;
            if (strcmp(sheet_cells[index].name, name) == 0) {
                if (sheet_cells[index].float_set) 
                    return sheet_cells[index].float_num;
                if (sheet_cells[index].int_set) 
                    return (double)sheet_cells[index].int_num;
                return 0.0; // Empty cell counts as 0
            }
        }
    }

    return 0.0;
}

double parse_factor() {
    skip_whitespace();
    double temp = 0.0;

    // Handle Numbers
    if (isdigit(*expression_ptr) || *expression_ptr == '.') {
        char *end_ptr;
        temp = strtod(expression_ptr, &end_ptr);
        expression_ptr = end_ptr;
    }
    // Handle Parentheses
    else if (*expression_ptr == '(') {
        expression_ptr++; // skip '('
        temp = parse_expression();
        skip_whitespace();
        if (*expression_ptr == ')') {
            expression_ptr++; // skip ')'
        } 
        else {
            printf("Error: Missing ')'\n");
            parse_error = 1;
        }
    }

    else if (isalpha(*expression_ptr)) {
        char token[20];
        int i = 0;
        // Read the word (e.g., "sin" or "A1")
        while (isalnum(*expression_ptr)) {
            if (i < 19) 
                token[i++] = *expression_ptr;
            expression_ptr++;
        }
        token[i] = '\0';

        skip_whitespace();
        
        // Check if it is a function
        if (*expression_ptr == '(') {
            expression_ptr++; // skip '('
            double arg = parse_expression();
            skip_whitespace();
            if (*expression_ptr == ')') 
                expression_ptr++;

            if (strcmp(token, "sin") == 0) 
                temp = sin(arg);

            else if (strcmp(token, "cos") == 0) 
                temp = cos(arg);

            else if (strcmp(token, "tan") == 0) 
                temp = tan(arg);

            else if (strcmp(token, "sqrt") == 0) 
                temp = sqrt(arg);

            else if (strcmp(token, "abs") == 0) 
                temp = fabs(arg);

            else if (strcmp(token, "log") == 0) 
                temp = log10(arg);

            else if (strcmp(token, "ln") == 0) 
                temp = log(arg);
            else {
                printf("Error: Unknown function '%s'\n", token);
                parse_error = 1;
            }
        }
        // It must be a cell reference like "A1"
        else {
            temp = get_cell_value_by_name(token);
        }
    }

    else if (*expression_ptr == '-') {
        expression_ptr++;
        return -parse_factor();
    }
    
    return temp;
}

double parse_term() {
    double left = parse_factor();
    skip_whitespace();
    
    while (*expression_ptr == '*' || *expression_ptr == '/' || *expression_ptr == '^') {
        char op = *expression_ptr;
        expression_ptr++;
        double right = parse_factor();
        
        if (op == '*') 
            left *= right;
        else if (op == '/'){
            if (right == 0){ 
                printf("Error: Division by zero\n"); 
                parse_error = 1; 
                return 0; 
            }
            left /= right;
        }
        else if (op == '^') 
            left = pow(left, right);
    }
    return left;
}

double parse_expression() {
    double left = parse_term();
    skip_whitespace();
    
    while (*expression_ptr == '+' || *expression_ptr == '-') {
        char op = *expression_ptr;
        expression_ptr++;
        double right = parse_term();
        
        if (op == '+') 
            left += right;
        else if (op == '-') 
            left -= right;
    }
    return left;
}

void recalculate_all(struct CELL_INFO *cells, int rows, int cols) {
    int index;
    int err;
    for (int k=0; k<3; k++) { // Repeat to resolve dependencies
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                index = r * cols + c;
                if (cells[index].formula_set == 1) {
                    double res = evaluate_formula(cells[index].formula, cells, rows, cols, &err);
                    if (!err) {
                        cells[index].float_num = res;
                        cells[index].float_set = 1;
                        cells[index].int_set = 0;
                        cells[index].status[0] = 2;
                    }
                }
            }
        }
    }
}

void skip_whitespace(){
    while (*expression_ptr == ' ') 
        expression_ptr++;
}