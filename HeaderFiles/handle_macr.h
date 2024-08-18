#ifndef C_PROJECT_OPENU_HANDLE_MACR_H
#define C_PROJECT_OPENU_HANDLE_MACR_H

#include <stdio.h>
#include "globals.h"
#include "macr_node.h"

/**
 * This function processes a file by removing extra spaces, building a list of macros, and replacing macros in the file.
 * It performs several operations on a file, including removing extra spaces, building a linked list of
 * macros, and replacing macro occurrences in the file. It returns the processed file name.
 * @param as_file  Pointer to the null-terminated string representing the name of the file to be processed.
 * @return A pointer to the null-terminated string representing the name of the processed file, or NULL if an error
 *         occurs during processing. The returned file name should be freed by the caller when no longer needed.
 *         On failure, the function cleans up any allocated resources and returns NULL.
 * @note The function performs the following steps:
 *       1. Removes extra spaces from the file content using `delete_extra_spaces_from_file`.
 *       2. Builds a linked list of macros using `build_macros_list`.
 *       3. Replaces macros in the file content using `replace_macros`.
 *       4. Cleans up the macro list with `free_nodes_list` and returns the processed file name.
 */
char *macr_pre_process(char as_file[]);

/**
 * @brief Builds a linked list of macro definitions from a file.
 *
 * This function builds a linked list of macro definitions from a file.
 * It reads a file to extract macro definitions and their associated code. It creates a linked list
 * of `macr_node` structures, where each node represents a macro with its name, code, and declaration line number.
 * @param head_macr Pointer to a pointer to the head of the linked list where the macro nodes will be stored.
 *                  The function updates this list with new macro definitions.
 * @param am_file   Pointer to the null-terminated string representing the name of the file to be processed.
 * @return 1 if the macro list is successfully built, or 0 if an error occurs.
 *         On failure, an appropriate error message is printed using `general_errors` or `is_valid_macr`.
 */
int build_macros_list(macr_node **head_macr,char *am_file);


/**
 * This function reads the code of a macro from a file. 
 * It reads lines from a file and accumulates them into a single string until it encounters the "endmacr" 
 * token. The accumulated content is returned as a dynamically allocated string. The function also handles memory 
 * allocation and reallocation as needed.
 * @param file_name    Pointer to the null-terminated string representing the name of the file being processed.
 * @param file         Pointer to the open file from which the macro code is read.
 * @param line_number  Pointer to an integer where the line number is updated as lines are read from the file.
 * @return A pointer to a dynamically allocated string containing the macro code, or NULL if an error occurs.
 *         On failure, an appropriate error message is printed using `general_errors` or `input_errors`.
 */
char* get_macr_code(char * file_name,FILE *file, int *line_number);

/**
 * This function extracts and validates the name of a macro from a line of input. It ensures that the macro name is valid,
 * not already used as an instruction, opcode, or register, and checks that the macro definition is properly formatted.
 * @param line       The line number where the macro is defined, used for error reporting.
 * @param am_file    Pointer to the null-terminated string representing the name of the file being processed.
 * @param macr_name  Pointer to a pointer where the validated macro name will be stored.
 *                   The function allocates memory for the name and assigns it to this pointer.
 * @return 1 if the macro name and definition are valid, or 0 if there are any errors.
 *         On failure, an appropriate error message is printed using `input_errors`.
 */
int is_valid_macr(int line, char *am_file, char **macr_name);

/**
 * This function checks for conflicts between macro names and labels in a specified file.
 * It reads the file line by line, looking for labels (defined as strings before a colon ':'). If a label matches
 * any macro name in the linked list, an error is reported, and the function returns 0. If no conflicts are found,
 * the function completes successfully and returns 1.
 * @param head_macr Pointer to the head of a linked list of macro nodes. Each node contains a macro name
 *                  (`macr_name`) to be checked against labels in the file.
 * @param am_file Pointer to the null-terminated string representing the name of the file to be checked.
 *                The file is opened and read line by line to identify potential label conflicts.
 * @return 1 if no conflicts are found; 0 if a conflict is detected or if the file cannot be opened.
 */
int ensure_macr_no_label(macr_node *head_macr, char *am_file);

/**
 * This function replaces macro definitions in a file with their corresponding code.
 * At first,it removes macro definitions from a file, then processes the file line by line,
 * replacing occurrences of macro names with their defined code. The modified content is written to
 * a temporary file, which is then used to overwrite the original file.
 * @param head_macr Pointer to the head of a linked list of macros. Each `macr_node` contains a macro
 *                  name and its corresponding code to replace in the file.
 * @param am_file   Pointer to the null-terminated string representing the name of the file to be processed.
 * @return 1 if the operation is successful, or 0 if an error occurs. 
 *         On failure, an appropriate error message is printed using `general_errors`.
 */
int replace_macros(macr_node *head_macr, char *am_file);

/**
 * This function removes macro definitions from a file by reading it line by line, 
 * removing all lines between "macr" and "endmacr" markers,
 * which are treated as macro blocks. It writes the cleaned content to a temporary file and then
 * overwrites the original file with this cleaned content.
 * @param filename Pointer to the null-terminated string representing the name of the file to be processed.
 * @return 1 if the operation is successful, or 0 if an error occurs. 
 *         On failure, an appropriate error message is printed using `general_errors`.
 */
int remove_macros(char *filename);

#endif