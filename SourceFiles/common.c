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
       general_errors("failed to allocate memory");
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
    char *src = str;  
    char *dest = str; 
    int in_space_sequence = 0;
    while (isspace(*src)) {
        src++;
    }

    while (*src != '\0') {
        if (isspace(*src)) {
            if (!in_space_sequence) {
                *dest++ = ' ';
                in_space_sequence = 1;
            }
        } else {
            *dest++ = *src;
            in_space_sequence = 0;
        }
        src++;
    }

    if (dest > str && *(dest - 1) == ' ') {
        dest--;
    }

    *dest++ = '\n';
    *dest = '\0';
    return str;
}

char *delete_extra_spaces_from_file(char as_file[]){
    char *output_file, *fixed_line;
    char buffer[INITIAL_BUFFER_SIZE], error_msg[INITIAL_BUFFER_SIZE];
    int line;
    FILE *file, *file_temp;
    file = fopen(as_file , "r");

    if (file==NULL){
        sprintf(error_msg,"Couldn't open file %s", as_file);
        general_errors(error_msg);
        return NULL;
    }

    output_file = generate_file_name(as_file, ".am");
    file_temp = fopen(output_file, "w");

    if (file_temp==NULL){
        fclose(file);
        remove(output_file);
        free(output_file);
        sprintf(error_msg,"Couldn't open file %s", as_file);
        general_errors(error_msg);    
        return NULL;
    }

    while (fgets(buffer, INITIAL_BUFFER_SIZE, file) != NULL) {
        line++;
        if(buffer[0]=='\n'){continue;}
        if (strlen(buffer) > MAX_LINE_LENGTH) {
            input_errors(as_file, line, "Line length isn't valid");
            fclose(file);
            fclose(file_temp);
            remove(output_file);
            free(output_file);
            return NULL;
        }
        else if (*buffer == ';') { /* Checks if it a comment line */
           continue;
        } else {
            /* removing extra unnecessary white-spaces from the line */
            fixed_line = delete_extra_spaces_from_str(buffer);
            if (*fixed_line == ';') { /* Checks if it a comment line after cleaning spaces*/
                continue;
            }
        }
        /* saving the changed line to the new file */
        fprintf(file_temp, "%s", fixed_line);

    }
    fclose(file);
    fclose(file_temp);
    return output_file;
}

void delete_extra_line(char *filnename){
    FILE *file, *temp;
    char line[MAX_LINE_LENGTH],error_msg[INITIAL_BUFFER_SIZE];
    char **lines = NULL;
    size_t count = 0, i;

    file = fopen(filnename, "r");
    if (file == NULL) {
        sprintf(error_msg,"Couldn't open file %s", filnename);
        general_errors(error_msg);
        return ;
    }

    while (fgets(line, sizeof(line), file)) {
        lines = realloc(lines, sizeof(char*) * (count + 1));
        if (lines == NULL) {
            general_errors("Failed to allocate memory");
            fclose(file);
            return ;
        }
        lines[count] = strdup(line);
        if (lines[count] == NULL) {
            sprintf(error_msg,"Failed to duplicate line %ld in file %s",count, filnename);
            general_errors(error_msg);
            fclose(file);
            return ;
        }
        count++;
    }
    fclose(file);

    temp = fopen(filnename, "w");
    if (temp == NULL) {
        sprintf(error_msg,"Couldn't open file %s", filnename);
        general_errors(error_msg);
        perror("Failed to open the file");
        return ;
    }

    for (i = 0; i < count - 1; i++) {
        fputs(lines[i], temp);
        free(lines[i]); 
    }
    lines[count-1][strlen(lines[count-1]) -1] = '\0'; 
    fputs(lines[count-1], temp);
    free(lines[count-1]); 
    free(lines); 

    fclose(temp);

    return ;
}