#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/globals.h"


void *manual_malloc(long object_size) {
    void *malloc_result = malloc(object_size);
    if (malloc_result == NULL) {
       printf("failed to allocate memory");
      /*print_internal_error(error_1);*/
    }
    return malloc_result;
}

char *generate_file_name(char *orig_name, char *new_end) {
    char *dot_position;
    char *new_file_name = manual_malloc(MAX_LINE_LENGTH * sizeof(char));
    size_t base_length;

    /* Find the position of the last '.' in the file name*/
    dot_position = strchr(orig_name, '.');
    if (dot_position) {
        base_length = dot_position - orig_name;
    } else {
        /* No '.' found, use the entire file name as the base*/
        base_length = strlen(orig_name);
    }
    /* Create the new file name using snprintf for safe concatenation*/
    snprintf(new_file_name, MAX_LINE_LENGTH, "%.*s%s", (int)base_length, orig_name, new_end);
    return new_file_name;
}

char *delete_extra_spaces_from_str(char str[]){
    
    int len = strlen(str);
    int x = 0, i;
    int spaceEncountered = 0;
    /* Allocate memory for the new string*/
    char *result = manual_malloc((len+1) * sizeof(char));

    /* Remove leading spaces*/
    for (i = 0; i < len && isspace(str[i]); i++);

    /* Traverse the input string*/
    for (; i < len; i++) {
        if (!isspace(str[i])) {
            /* Copy non-space characters*/
            result[x++] = str[i];
            spaceEncountered = 0;
        } else if (!spaceEncountered) {
            /* Copy a single space if not a leading space*/
            result[x++] = ' ';
            spaceEncountered = 1;
        }
    }

    /* Remove trailing spaces*/
    if (x > 0 && result[x-1] == ' ') {
        x--;
    }

    /* Null-terminate the resulting string*/
    result[x] = '\n';

    return result;
}

char *delete_extra_spaces_from_file(char as_file[]){
    char *output_file, *fixed_line;
    char buffer[1000];
    int line;
    FILE *file, *file_temp;
    file = fopen(as_file , "r");

    if (file==NULL){
        general_errors("Couldn't open file");
        return NULL;
    }

    output_file = generate_file_name(as_file, ".tmp");
    file_temp = fopen(output_file, "w");

    if (file_temp==NULL){
        fclose(file);
        remove(output_file);
        free(output_file);
        general_errors("Couldn't open file");       
        return NULL;
    }

    while (fgets(buffer, 999, file) != NULL) {
        line++;
        if (strlen(buffer) > MAX_LINE_LENGTH) {
            input_errors(as_file, line, "Line length isn't valid");
            fclose(file);
            fclose(file_temp);
            remove(output_file);
            free(output_file);
            return NULL;
        }
            /* replacing a comment line with newline character */
        else if (*buffer == ';') {
           continue;
        } else {
            /* removing extra unnecessary white-spaces from the line */
            fixed_line = delete_extra_spaces_from_str(buffer);
        }
        /* saving the changed line to the new file */
        fprintf(file_temp, "%s", fixed_line);

    }
    fclose(file);
    fclose(file_temp);
    return output_file;
}



    
    





