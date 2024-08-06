#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/handle_macr.h"
#include "../HeaderFiles/tables.h"


void first_second_pass(char *dot_am_file){
    int DC=0, IC=-1, line=0, entries_cnt=0, externs_cnt = 0;
    FILE *file;
    char buffer[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE] = {0};
    gen_label_table *entries_table, *externs_table; 
    instruction *inst;

    entries_table = manual_malloc(sizeof(gen_label_table));
    if (entries_table == NULL) {
        return ;
    }

    externs_table = manual_malloc(sizeof(gen_label_table));
    if (externs_table == NULL) {
        return ;
    }

    file = fopen(dot_am_file , "r");
    if (file==NULL){
        sprintf(error_msg,"Couldn't open file %s", dot_am_file);
        general_errors(error_msg);
        return ;
    }
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL && IC + DC <= MAX_IC - IC_INIT_VALUE) {
        line++;
        memset(error_msg, 0, sizeof(error_msg));
        if (strchr(buffer, '.')) {
            if (strstr(buffer, ".entry")){
                inst = create_instruction(buffer, 1, error_msg);
                if (inst == NULL){
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                    }
                    continue;
                }
                else{insert_general_label(&entries_table, ++entries_cnt, inst, line);}   
            }
            else if (strstr(buffer, ".extern")){
                inst = create_instruction(buffer, 0, error_msg);
                if (inst == NULL){
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                    }
                    continue;
                }
                else{insert_general_label(&entries_table, ++entries_cnt, inst, line);}   
            }
            else if (strstr(buffer, ".data") != NULL || strstr(buffer, ".string") != NULL) {
                inst = create_instruction2(buffer, error_msg); /*IM HERE */
                if (inst == NULL) {
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                    }
                    continue;
                }
                else{insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1, &error_code);}
            }
        }
    }
}