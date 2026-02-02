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

This project presents the design and implementation of a **lightweight spreadsheet simulation engine** written in the C programming language.  

The system supports **cell-based value storage** inspired by the core computational logic of spreadsheet software such as Microsoft Excel.

The project is designed as a foundation for future expansion toward expression parsing and formula computation.

---

## 1. Project Overview

The system simulates a table of cells similar to spreadsheet software, where each cell:

- Has a unique name (e.g., A1, B2)
- Can store numeric values
- Can be accessed and modified via textual commands

All interactions with the system are performed through a **command-based interface**, making the program suitable for testing internal logic without a graphical layer.

---

## 2. Architecture and Modular Design

The codebase is structured modularly to separate concerns:

├── common.h # Shared data structures and definitions

├── sheet.c # Cell creation, access, and value assignment

├── fileio.c # File-related operations (save/load)

├── main.c # Command parsing and program control flow

This modularization improves:
- Readability
- Maintainability
- Future extensibility (especially for formula support)

---

## 3. Data Structures

Each cell is represented using a dedicated structure containing:

- Cell identifier
- Value type
- Stored numeric value

This explicit modeling mirrors real spreadsheet engines at a conceptual level and avoids ambiguous data handling.

---

## 4. Command-Based Interface (Core Feature)

One of the key aspects of this project is the **command interpreter**, which allows users to interact with the spreadsheet using textual commands.

Examples of supported operations include:

- Creating or accessing cells
- Setting numeric values
- Displaying cell information
- Managing the table structure

The command system is designed to be:
- Deterministic
- Easily extendable
- Independent of internal data representation

This design choice allows new commands (e.g., formulas, functions) to be added without refactoring existing logic.

---

## 5. Current Capabilities

✔ Cell creation and indexing  
✔ Numeric value storage  
✔ Structured command parsing  
✔ Modular code organization  

---

## License

This project is intended for educational use.