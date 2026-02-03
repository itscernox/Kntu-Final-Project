# Spreadsheet Simulator in C

## A Command-Based Cell Management System

---

### Author  
**Mahyar Mousavi**  

### Affiliation  
**K. N. Toosi University of Technology (KNTU) – Department of Electrical Engineering**  

### Course  
**Basic C Programming** 

---

## Abstract

This project presents the design and implementation of a console‑based spreadsheet simulation engine written in the C programming language.

The system emulates the core computational behavior of spreadsheet software (such as Microsoft Excel) by supporting:

- Cell‑based numeric storage
- Dynamic sheet resizing
- Persistent binary storage
- Mathematical formula evaluation with cell references

---

## 1. Project Overview

The Spreadsheet Engine Simulation System (SESS) models a spreadsheet as a 2D grid of cells, where each cell:

- Has a unique identifier (e.g., A1, B3)
- Stores either an integer or floating‑point value
- May optionally contain a mathematical formula
- Automatically updates its value when dependencies change

All interactions with the system are performed through a **command-based interface**, making the program suitable for testing internal logic without a graphical layer.

---

## 2. Architecture and Modular Design

The codebase follows a clear separation of concerns, with each module responsible for a specific subsystem:

├── common.h     # Shared structures, constants, and includes
├── main.c       # Program entry point and command interpreter
├── sheet.c      # Sheet manipulation and cell operations
├── fileio.c     # Binary persistence and export utilities
├── formula.c    # Formula parsing and evaluation engine
├── *.h          # Interface definitions for each module

This modularization improves:
- Readability
- Maintainability
- Future extensibility (especially for formula support)

---

## 3. Data Structures

Each spreadsheet cell is represented using the following structure:

- Cell name (A1, B2, …)
- Integer value and status
- Floating‑point value and status
- Optional formula string
- Type/status indicator (integer or float)

This explicit modeling closely mirrors real spreadsheet engines and avoids ambiguous value handling.

---

## 4. Command-Based Interface (Core Feature)

The system uses a deterministic command interpreter that processes user input at runtime.

Supported Commands

1 - resize sheet (rs) : Resize the spreadsheet (rows/columns)
2 - show cells (sc) : Display the sheet layout
3 - set value (sv) : Assign a numeric value to a cell
4 - set formula (sf) : Assign and evaluate a formula
5 - cell info (cf) : Display detailed cell information
6 - help (hp) : Display command list
7 - save (s) : Persist current state to disk
8 - exit : Terminate the program

The command system is designed to be easily extensible, allowing new operations to be added without refactoring existing logic.

---

## 5. File Persistence

5.1 Binary Storage
The spreadsheet state is stored in a binary file, including:

- Sheet dimensions
- Full cell data structures
- Stored formulas and values

This allows the spreadsheet to be restored exactly as it was at program startup.

5.2 Text Export
Upon program termination, a human‑readable report is automatically generated, summarizing:

- Cell names
- Data types
- Final computed values
- Timestamp of export
---

## 6. Current Capabilities

✔ Dynamic sheet resizing

✔ Integer and floating‑point cell values

✔ Command‑driven interaction

✔ Binary save/load

✔ Formula parsing and evaluation

✔ Automatic recalculation

✔ Modular and extensible design

---
## License

This project is intended for educational use.