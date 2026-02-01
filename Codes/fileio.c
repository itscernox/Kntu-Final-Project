#include "common.h"

int new_sheet(CELL_INFO **cells , int *rows , int *cols , FILE **f){

    printf("Creating a new spreadsheet...\n\n");
    delay_seconds(1);
    printf("Default sheet size is set to 5 rows x 5 columns.\n\n");

    // New sheet default setting
    *rows = 5; 
    *cols = 5;

    *cells = (CELL_INFO *) malloc ((*rows) * (*cols) * sizeof(CELL_INFO));
    if (!(*cells)){ 
        printf("Error : Memory Allocation Failed!\n"); 
        return 0; 
    }

    identify_cells(*cells , *rows , *cols);

    // Build Binary File (db)
    *f = fopen("/Users/cernox/Desktop/db" , "wb");
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
            //(*cells)[index].formula_set = 0;
            (*cells)[index].status[0] = 1;
            fwrite(&(*cells)[index] , sizeof(CELL_INFO) , 1 ,*f);
        }
    }

    fclose(*f);
    *f = fopen("/Users/cernox/Desktop/db" , "rb+");

    delay_seconds(1);
    printf("New spreed sheet created. Binary File Name : db\n\n");
    delay_seconds(1);

    return 1;
}

int load_binary(char *filename , CELL_INFO **cells , int *rows , int *cols , FILE **f){
    //*f = fopen(filename , "rb");
    *f = fopen("/Users/cernox/Desktop/db" , "rb");

    delay_seconds(1);
    printf("\nLoading spreadsheet...\n");
    delay_seconds(1);

    if (!(*f))
        return 0;

    // Read num of rows & cols from imported binary file
    fread(rows , sizeof(int) , 1 , *f);
    fread(cols , sizeof(int) , 1 , *f);

    *cells = (CELL_INFO *) malloc ((*rows) * (*cols) * sizeof(CELL_INFO));
    if (!(*cells)) { 
        printf("Error : Memory Allocation Failed!\n"); 
        return 0; 
    }

    // Read cells from imported binary file
    int index;
    for(int r = 0 ; r < *rows ; r++)
        for (int c = 0 ; c < *cols ; c++){
            index = r * (*cols) + c;
            fread(&(*cells)[index] , sizeof(CELL_INFO) , 1 , *f);
        }

    fclose(*f);
    *f = fopen("/Users/cernox/Desktop/db" , "rb+");
    
    printf("\nSpreed Sheet Imported Successfully\n\n");
    delay_seconds(1);

    return 1;
}

void save(FILE *f , CELL_INFO * cells, int rows , int cols){
    // Rewrite changes over file
    fseek(f , 0 , SEEK_SET);
    fwrite(&rows , sizeof(int) , 1 , f);
    fwrite(&cols , sizeof(int) , 1 , f);
    int index;
    for (int r = 0 ; r < rows ; r++)
        for (int c = 0 ; c < cols ; c++){
            index = r * cols + c;
            fwrite(&(cells[index]) , sizeof(CELL_INFO) , 1 , f);
        }
}

void export_txt(CELL_INFO *cells , int rows , int cols){

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