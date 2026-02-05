/*
 * ( Formula Engine )
 *
 * USED ALGORITHM : Recursive Descent Parser
 * 
 * DESCRIPTION:
 * This module implements a top-down Recursive Descent Parser to evaluate mathematical 
 * expressions. The parser treats the input string as a set of recursive grammar rules 
 * (Expression -> Term -> Factor), allowing it to naturally handle operator precedence 
 * (e.g., multiplication before addition) and parentheses without constructing an 
 * explicit Abstract Syntax Tree (AST) in memory.
 * 
 * Used EDUCATIONAL RESOURCES (VIDEO):
 * 1. Computerphile - "Parsing Formulae (Recursive Descent)" 
 *    [Search Query: Computerphile Recursive Descent Parsing]
 * 
 * 2. "Writing a Simple Math Expression Engine in C" (various authors on YouTube)
 *    [Search Query: C recursive descent parser tutorial]
 * 
 * IMPLEMENTATION NOTE & AI ASSISTANCE:
 * Due to the complexity of writing a parser from scratch, the core implementation logic for the Recursive Descent algorithm were
 * derived from educational YouTube resources d(e.g., Computerphile, CodeAesthetics) & assistance of [Google Gemini AI].
 *
 * !! BUT The logic is reviewed !! 
 * 
 * - MAHYAR MOUSAVI
*/

#include "common.h"

double evaluate_formula(char *formula_str, CELL_INFO *cells, int r, int c, int *error) {
    
    expression_ptr = formula_str;
    sheet_cells = cells;
    sheet_rows = r;
    sheet_cols = c;
    parse_error = 0;
    
    // Check for empty formula
    if (formula_str == NULL || strlen(formula_str) == 0) 
        return 0.0;
    
    // Skip '=' if present
    if (*expression_ptr == '=') 
        expression_ptr++;
    
    double result = parse_expression();
    
    if (parse_error) 
        *error = 1;

    else 
        *error = 0;
    
    return result;
}

double parse_expression(){
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

double parse_factor() {
    skip_whitespace();
    double temp = 0.0;

    // Handle Numbers
    if (isdigit(*expression_ptr) || *expression_ptr == '.') {
        char *end_ptr;
        temp = strtod(expression_ptr, &end_ptr); // strtod : string to double
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

    // Handle Functions (sin, cos) and Variables (A1)
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


void recalculate_all(CELL_INFO *cells, int rows, int cols) {
    int index;
    int err;
    // If formula_set is 1, re-evaluate.
    // Running this loop 2-3 times solves most dependencies.
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
    while (*expression_ptr == ' ') expression_ptr++;
}