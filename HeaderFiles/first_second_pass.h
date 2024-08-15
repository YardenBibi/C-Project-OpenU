#ifndef C_PROJECT_OPENU_FIRST_SECOND_PASS_H
#define C_PROJECT_OPENU_FIRST_SECOND_PASS_H

/**
 * Processes an assembly source file in two passes. The first pass handles directives and commands 
 * to build symbol tables and machine code. The second pass updates label addresses and generates 
 * output files. This function also manages memory allocation and error handling throughout the process.
 * @param dot_am_file The name of the input assembly source file to be processed.
 * The function performs the following steps:
 * 1. Initializes memory for various tables and arrays.
 * 2. Opens the specified assembly source file for reading.
 * 3. **First Pass**:
 *    - Handles directives like `.entry`, `.extern`, `.data`, and `.string` to build the symbol tables
 *      and generate intermediate machine code.
 *    - Processes commands, converts them to machine code, and updates the tables.
 * 4. **Second Pass**:
 *    - Updates label addresses based on the final values and merges instruction lines.
 *    - Resolves external labels and handles label references.
 * 5. **Output and Reporting**:
 *    - Generates output files (`.ob`, `.ext`, `.ent`) if no errors are present.
 *    - Prints debugging information for verification.
 * 6. **Memory Management**:
 *    - Frees all allocated memory for tables and arrays to avoid memory leaks.
 */
void first_second_pass(char *dot_am_file);

#endif