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
    int DC=0, IC=-1, line=0, entries_cnt=0, externs_cnt = 0, labels_cnt=0;
    FILE *file;
    char buffer[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE] = {0};
    unsigned short cmd_as_short;
    gen_label_table *entries_table, *externs_table; 
    label_table *label_tbl;
    instruction *inst;
    command *cmd;
    converted_line *inst_lines, *code_lines;

    entries_table = manual_malloc(sizeof(gen_label_table));
    if (entries_table == NULL) {
        return ;
    }

    externs_table = manual_malloc(sizeof(gen_label_table));
    if (externs_table == NULL) {
        return ;
    }
    
    label_tbl = manual_malloc(sizeof(label_table));
    if (label_tbl == NULL) {
        return ;
    }

    inst_lines = manual_malloc(sizeof(converted_line));
    if (inst_lines == NULL) {
        return ;
    }
    code_lines = manual_malloc(sizeof(converted_line));
    if (code_lines == NULL) {
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
                else{insert_general_label(&externs_table, ++externs_cnt, inst, line);}   
            }
            else if (strstr(buffer, ".data") != NULL || strstr(buffer, ".string") != NULL) {
                inst = create_instruction2(buffer, error_msg); 
                if (inst == NULL) {
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                    }
                    continue;
                }
                if (!insert_label_table(&label_tbl, ++labels_cnt, inst->inst_label, DC, line, 1, error_msg)){
                    general_errors(error_msg);
                    continue;
                }
            }
            else {
                input_errors(dot_am_file,line,"Illegal line with .");
                continue;
            }
            add_machine_code_inst(&inst_lines, inst, &DC, line);
            if (inst->nums){
                free(inst->nums);
            }
            free(inst);
            continue;
        }
        cmd = create_command(buffer, error_msg);
        if(cmd == NULL){
            if (strlen(error_msg)!=0){
                input_errors(dot_am_file,line,error_msg);
            }
            continue;
        }
        IC++;
        if (cmd->cmd_label != NULL) {
            if (!insert_label_table(&label_tbl, ++labels_cnt, cmd->cmd_label, IC, line, 0, error_msg)){
                general_errors(error_msg);
                continue;
            }
        }
        cmd_as_short = command_to_short(&externs_table,externs_cnt, cmd);
        if (add_machine_code_cmd(&code_lines, cmd_as_short, NULL, &IC, line) == 0) {
                free(cmd);
                general_errors("Couldn't update machine code table");
                continue;
        }
        if (add_information_line(&code_lines, cmd, &IC, 1, line) == 0 || add_information_line(&code_lines, cmd, &IC, 0, line) == 0) {
                free(cmd);
                general_errors("Couldn't update machine code table");
                continue;
        }
        free(cmd);
    }

    /*SECOND PASS*/

    
}