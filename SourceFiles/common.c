#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *manual_malloc(long object_size) {
    void *malloc_result = malloc(object_size);
    if (malloc_result == NULL) {
       printf("failed to  allocate memory");
      //print_internal_error(error_1);
    }
    return malloc_result;
}

char *generate_file_name(char *orig_name, char *new_end) {
    char *dot_position;
    char *new_file_name = manual_malloc(MAX_LINE_LENGTH * sizeof(char));
    size_t base_length;

    // Find the position of the last '.' in the file name
    dot_position = strchr(orig_name, '.');
    if (dot_position) {
        base_length = dot_position - orig_name;
    } else {
        // No '.' found, use the entire file name as the base
        base_length = strlen(orig_name);
    }
    // Create the new file name using snprintf for safe concatenation
    snprintf(new_file_name, MAX_LINE_LENGTH, "%.*s%s", (int)base_length, orig_name, new_end);
    return new_file_name;
}

char *delete_extra_spaces(char as_file[]){
    return; 
}

