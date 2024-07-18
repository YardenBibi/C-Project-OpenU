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

char *delete_extra_spaces(char as_file[]);
