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
 * structure of this algorithm were derived from educational YouTube resources d(e.g., Computerphile, CodeAesthetics) and generated with the assistance of 
 *
 * !! BUT The logic is reviewed !! 
 * 
 * - MAHYAR MOUSAVI
*/

#ifndef FORMULA_H
#define FORMULA_H

#include "common.h"

// Global Variables
const char *expression_ptr;

CELL_INFO *sheet_cells;

int sheet_rows;

int sheet_cols;

int parse_error; // 0: OK, 1: Error

// Returns the result and sets *error if something goes wrong
double evaluate_formula(char *, CELL_INFO *, int , int , int *);

// Find value of a cell by name
double get_cell_value_by_name(char *);

// Handles numbers, parentheses, functions, variables (A1)
double parse_factor();

// Handles * , / , ^
double parse_term();

// Handles + , -
double parse_expression();

// Recalculate results of formula when a value of cell will change by set value command
void recalculate_all(CELL_INFO *, int , int);

// Skip spaces
void skip_whitespace();

#endif