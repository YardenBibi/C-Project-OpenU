#ifndef C_PROJECT_OPENU_COMMON_H
#define C_PROJECT_OPENU_COMMON_H

#include "globals.h"

/**
 * This function allocates a memory block and displays an error in case the process fails.
 * @param object_size - the size of the memory block to allocate
 * @return a void pointer to the allocated memory, or NULL if the allocation fails.
 */
void *manual_malloc(long object_size);

/**
 * This function generates a new file name, If the original file name does not contain a '.', the entire file name is used as the base.
 * @param orig_name The original file name
 * @param new_end The new extension or suffix to be appended to the base name
 * @return A pointer to the newly allocated string containing the new file name
 */
char *generate_file_name(char *orig_name, char *new_end);

/**
 * This function removes extra spaces from a string and returns a new string with condensed spaces.
 * @param str Pointer to the null-terminated input string. The function modifies the string to remove extra spaces.
 * @return Pointer to a newly allocated string with extra spaces removed. The caller is responsible for
 *         freeing this memory.
 */
char *delete_extra_spaces_from_str(char str[]);

/**
 * This function reads a file line by line, removes extra spaces from each line, and writes the cleaned lines
 * to a new file with a modified name. Comment lines (starting with ';') are skipped, and lines longer than
 * `MAX_LINE_LENGTH` are considered invalid.
 *
 * @param as_file Pointer to the null-terminated string representing the name of the file to be processed.
 *                The function will generate a new file name with a ".am" extension for output.
 * @return Pointer to the newly created file name with the ".am" extension.
 *         If an error occurs, the function handles it by calling `general_errors` and returns `NULL`.
 */
char *delete_extra_spaces_from_file(char as_file[]);

#endif
