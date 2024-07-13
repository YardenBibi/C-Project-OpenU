#ifndef C_PROJECT_OPENU_COMMON_H
#define C_PROJECT_OPENU_COMMON_H

#include "globals.h"

/**
 * This function allocates new memory and handles the errors that might occur
 * @param object_size the amount of memory to allocate
 * @return a void pointer of the allocated memory, or NULL if the allocation failed
 */
void *(long object_size);

/**
 * This function saves a new name for a file. It deletes the content of the name after the '.' if one exists
 * and adds a new ending
 * @param file_name string of the beginning of the new file name
 * @param ending string of the ending of the new file name
 * @return a string with the new file name
 */
char *add_new_file(char *file_name, char *ending);
