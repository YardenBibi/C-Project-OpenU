#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/handle_macr.h"

int remove_macros(char *filename) {
    char line[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE];
    int in_macro_block = 0;

    FILE *file,*temp_file;
    file = fopen(filename, "r");
    if (!file) {
        sprintf(error_msg,"Couldn't open file %s", filename);
        general_errors(error_msg);
        return 0;
    }

    temp_file = tmpfile();
    if (!temp_file) {
        general_errors("Failed to create temporary file");
        fclose(file);
        return 0;
    }


    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "macr", 4) == 0) {
            in_macro_block = 1;
        }

        if (strncmp(line, "endmacr", 7) == 0) {
            in_macro_block = 0;
            continue; 
        }

        if (!in_macro_block) {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    rewind(temp_file);

    file = fopen(filename, "w");
    if (!file) {
        sprintf(error_msg,"Couldn't open file %s", filename);
        general_errors(error_msg);
        fclose(temp_file);
        return 0;
    }

    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, file);
    }

    fclose(file);
    fclose(temp_file);
    return 1;
}


int replace_macros(macr_node *head_macr, char *no_extra_space_file){
    char line[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE];
    int replaced;
    macr_node *macr;
    FILE *file,*temp_file;
    if (!remove_macros(no_extra_space_file)){
        return 0;
    }
    file = fopen(no_extra_space_file, "r");
    if (!file) {
        sprintf(error_msg,"Couldn't open file %s", no_extra_space_file);
        general_errors(error_msg);
        return 0;
    }

    temp_file = tmpfile();
    if (!temp_file) {
        general_errors("Failed to create temporary file");
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        replaced = 0;
        macr = head_macr;
        while (macr != 0)
        {
            if (strncmp(line, macr->macr_name, strlen(macr->macr_name)) == 0 && (line[strlen(macr->macr_name)] == '\0' || line[strlen(macr->macr_name)] == '\n')) {
                fputs(macr->macr_code, temp_file);
                replaced = 1;
                break;
            }
            macr = macr->next;

        }
        if (!replaced) {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    rewind(temp_file);

    file = fopen(no_extra_space_file, "w");
    if (!file) {
        sprintf(error_msg,"Couldn't open file %s", no_extra_space_file);
        general_errors(error_msg);
        fclose(temp_file);
        return 0;
    }

    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, file);
    }

    fclose(file);
    fclose(temp_file);
    return 1;
}


int ensure_macr_order(macr_node *head_macr, char *no_extra_space_file){
    char buffer[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE];
    int line=0;
    FILE *file; 
    macr_node *macr;

    file = fopen(no_extra_space_file , "r");
    if (file==NULL){
        sprintf(error_msg,"Couldn't open file %s", no_extra_space_file);
        general_errors(error_msg);
        return 0;
    }

    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        line++;
        if (strstr(buffer, "macr") != NULL) {
            continue;
        }  
        macr = head_macr;
        while (macr != NULL)
        {
            if (strstr(buffer, macr->macr_name) != NULL && line<macr->macr_row){
                sprintf(error_msg,"The macro '%s' is called before it's decleration.\n", macr->macr_name);
                input_errors(no_extra_space_file,line,error_msg);
                return 0;
            }
            macr = macr->next;

        }
        
    }

    return 1;
}

int build_macros_list(macr_node **head_macr, char *no_extra_space_file){
    char buffer[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE];
    int line=0, current_macr_line;
    FILE *file; 
    char *macr_name, *macr_code; 

    file = fopen(no_extra_space_file , "r");
    if (file==NULL){
        sprintf(error_msg,"Couldn't open file %s", no_extra_space_file);
        general_errors(error_msg);
        return 0;
    }
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        line++;
        if (strcmp(strtok(buffer, " "), "macr") == 0){
            current_macr_line = line;
            if (!is_valid_macr(line, no_extra_space_file,&macr_name)){
                fclose(file);
                return 0;
            }
            
            macr_code = get_macr_code(no_extra_space_file,file,&line);
            if (macr_code==NULL){
                fclose(file);
                return 0; 
            }

            if (add_node(no_extra_space_file, head_macr, macr_name, macr_code,current_macr_line) == -1){
                fclose(file);
                return 0; 
            }
        }
    }
    fclose(file);
    return 1;
    
}

int is_valid_macr(int line, char *no_extra_space_file, char **macr_name){
    char *name; 
    name=strtok(NULL, " \n");
    if (name==NULL){
        input_errors(no_extra_space_file, line, "macro witout name");
        return 0;
    }
    *macr_name = manual_malloc((strlen(name)+1));
    if (name_as_inst(name)>0 || which_opcode(name)>=0 || which_reg(name)>=0){
        input_errors(no_extra_space_file, line, "Macro name isn't valid");
        return 0;
    }
    
    if (strtok(NULL, "\n")!=NULL){
        input_errors(no_extra_space_file, line, "Macro definition isn't valid");
        return 0;
    }
    
    strcpy(*macr_name,name);
    return 1;
}

char* get_macr_code(char * file_name,FILE *file, int *line_number) {
    size_t content_size = INITIAL_BUFFER_SIZE;
    size_t content_length = 0;
    size_t line_length;
    char line[256];
    char *new_content;
    char *content = malloc(INITIAL_BUFFER_SIZE);
    
    if (content == NULL) {
        general_errors("Failed to allocate memory");
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        (*line_number)++; 
        
        if (strcmp(line, "endmacr\n") == 0) {
            break;
        }

        if ((strstr(line, "endmacr") != NULL)){
            input_errors(file_name, *line_number, "end_macr token with extra characters");
            return NULL;
        }
        
        line_length = strlen(line);
        if (content_length + line_length + 1 > content_size) {
            content_size *= 2;
            new_content = realloc(content, content_size);
            if (new_content == NULL) {
                general_errors("Failed to reallocate memory");
                free(content);
                return NULL;
            }
            content = new_content;
        }
        strcpy(content + content_length, line);
        content_length += line_length;
    }
    content[content_length] = '\0';

    return content;
}

int macr_pre_process(char as_file[]){
    char *no_extra_space_file;
    macr_node *head_macr = NULL;
    no_extra_space_file = delete_extra_spaces_from_file(as_file);
    if (no_extra_space_file==NULL)
        return 0;
    if (!build_macros_list(&head_macr,no_extra_space_file)){
        free_nodes_list(head_macr);
        return 0;
    }
    /*
    if (!ensure_macr_order(head_macr,no_extra_space_file)){
        free_nodes_list(head_macr);
        return 0;
    }
    */
    if (!replace_macros(head_macr,no_extra_space_file)){
        return 0;
    }

    free_nodes_list(head_macr);
    return 1;
    
}
