#include "common.h"

// Check operator to call appopriate function
void check_operator(char [] , int *  , int * , CELL_INFO ** , FILE * , int *);

int main(){

    // Initialization Mode
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
    CELL_INFO *cells;
    int rows , cols;

    // Import spreed sheet
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

    // New spreed sheet
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

    // Input opt by User
    char opt[50];

    // Check File is saved or not
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
                            printf("\n      Saved! Binary File updated succesfully");
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

    // txt file to report sheet status when user quit the programm
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

    // Terminate Program
    delay_seconds(1);
    printf("Terminating the program....");
    delay_seconds(1);
    return 0;
}

void check_operator(char opt[] , int * rows , int * cols , CELL_INFO **cells , FILE *f , int *is_save){
    
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
                    // Result is always float
                    double res = evaluate_formula(input_formula, *cells, *rows, *cols, &errno);
                    
                    if (errno == 0) {
                        (*cells)[index].float_num = res;
                        (*cells)[index].float_set = 1;
                        (*cells)[index].int_set = 0;
                        (*cells)[index].status[0] = 2;
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