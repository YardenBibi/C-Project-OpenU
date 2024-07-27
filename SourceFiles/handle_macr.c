#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/macr_node.h"

int build_macr_list(node **head_macr, char *no_extra_space_file){
    char buffer[MAX_LINE_LENGTH];
    int line=0, current_macr_line;
    FILE *file; 
    char *macr_name, *macr_code; 
    file = fopen(no_extra_space_file , "r");
    if (file==NULL){
        general_errors("Couldn't open file");
        return 0;
    }
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        line++;
        if (strcmp(strtok(buffer, " "), "macr") == 0){
            current_macr_line = line;
            macr_name=is_valid_macr(buffer, line, no_extra_space_file)
            if(macr_name==NULL){
                fclose(file);
                return 0;
            }

        }
        
        macr_code = get_macr_code(file);
        if (macr_code==NULL){
            fclose(file);
            return 0; 
        }

        /*create node*/
    }   
    fclose(file);
    return 1;
}

char *is_valid_macr(char *buffer, int line, char *no_extra_space_file){
    char *macr_name ; 
    macr_name=strtok(NULL, " \n");
    if (macr_name==NULL){
        input_errors(no_extra_space_file, line, "macro witout name");
        return NULL;
    }

    if (name_as_inst(macr_name) || which_opcode(macr_name)=>0 || which_reg(macr_name)=>0){
        input_errors(no_extra_space_file, line, "Macro name isn't valid");
        return NULL;
    }
    if (strtok(NULL, "\n")!=NULL){
        input_errors(no_extra_space_file, line, "Macro definition isn't valid");
        return NULL;
    }
    return macr_name;
}

char *get_macr_code(FILE file, int *line){


}

int macr_pre_process(char as_file[]){
    char *no_extra_space_file;
    macr_node *head_macr = NULL;
    no_extra_space_file = delete_extra_spaces_from_file(as_file);
    printf("No extra spaces file: %s\n", no_extra_space_file);
    if (no_extra_space_file==NULL)
        return 0;

    if(!build_macr_list(&head_macr,no_extra_space_file)){
        return 0;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 1;
    
}
