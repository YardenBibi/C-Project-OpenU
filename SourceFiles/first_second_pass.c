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
    int DC=0, IC=-1, line=0, entries_cnt=0, externs_cnt = 0, labels_cnt=0, success = 1;
    unsigned short n_are=4;
    FILE *file;
    char buffer[MAX_LINE_LENGTH], error_msg[INITIAL_BUFFER_SIZE] = {0}, copy[MAX_LINE_LENGTH];
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
    
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL && IC + DC <= MAX_IC - IC_INIT) {
        n_are=4;
        line++;
        if(buffer[0]=='\n'){continue;}
        memset(error_msg, 0, sizeof(error_msg));
        if (strchr(buffer, '.')) {
            if (strstr(buffer, ".entry")){
                inst = create_instruction(buffer, 1, error_msg);
                if (inst == NULL){
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                        success = 0;
                    }
                    continue;
                }
                else{
                    strcpy(copy,inst->arg_label);
                    insert_general_label(&entries_table, ++entries_cnt, inst, line,copy);
                    }   
            }
            else if (strstr(buffer, ".extern")){
                inst = create_instruction(buffer, 0, error_msg);
                if (inst == NULL){
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                        success = 0;
                    }
                    continue;
                }
                else{
                    strcpy(copy,inst->arg_label);
                    insert_general_label(&externs_table, ++externs_cnt, inst, line,copy);
                    }   
            }
            else if (strstr(buffer, ".data") != NULL || strstr(buffer, ".string") != NULL) {
                inst = create_instruction2(buffer, error_msg); 
                if (inst == NULL) {
                    if (strlen(error_msg)!=0){
                        input_errors(dot_am_file,line,error_msg);
                        success = 0;
                    }
                    continue;
                }
                if (inst->inst_label !=NULL){strcpy(copy,inst->inst_label);}
                if (inst->inst_label !=NULL && !insert_label_table(&label_tbl, ++labels_cnt, copy, DC, line, 1, error_msg)){
                    labels_cnt--;
                    general_errors(error_msg);
                    success = 0;
                    continue;
                }
            }
            else {
                input_errors(dot_am_file,line,"Illegal line with .");
                success = 0;
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
                success = 0;
            }
            continue;
        }
        IC++;
        if (cmd->cmd_label != NULL) {
            if (!insert_label_table(&label_tbl, ++labels_cnt, cmd->cmd_label, IC, line, 0, error_msg)){
                labels_cnt--;
                general_errors(error_msg);
                success = 0;
                continue;
            }
        }
        cmd_as_short = command_to_short(&externs_table,externs_cnt,&entries_table,entries_cnt, cmd,&n_are);
        if (add_machine_code_cmd(&code_lines, cmd_as_short, NULL, &IC, line) == 0) {
                free(cmd);
                general_errors("Couldn't update machine code table");
                success = 0;
                continue;
        }
        if (add_information_line(&code_lines, cmd, &IC, 1, line,n_are) == 0 || add_information_line(&code_lines, cmd, &IC, 0, line,n_are) == 0) {
                free(cmd);
                general_errors("Couldn't update machine code table");
                success = 0;
                continue;
        }
        free(cmd);
    }

    if (IC + DC > MAX_IC - IC_INIT) { /* Means we got out of the loop due to memory limitations*/
        sprintf(error_msg, "IC+DC size %d exceeded limit of %d",IC+DC,MAX_IC - IC_INIT);
        general_errors(error_msg);
        return ;
    }

    /*SECOND PASS*/

    update_labels(label_tbl,labels_cnt,IC);
    merge_all_lines(&code_lines, inst_lines, IC, DC);
    handle_externs_address(code_lines, externs_table, externs_cnt, IC + DC);
    handle_labels(code_lines, label_tbl, labels_cnt, IC, dot_am_file,error_msg);

    if(success && strlen(error_msg)==0){ /*Condtion for success run - creating output files*/
        create_ob_file(code_lines,IC,DC,dot_am_file);
        create_ext_file(code_lines,IC+DC,externs_table, externs_cnt, dot_am_file);
        create_ent_file(entries_table, entries_cnt,label_tbl, labels_cnt, dot_am_file);
        printf("SUCCESS\n");
    }
    
    /*FREE MEMORY*/

    free_machine_code(code_lines,IC+DC);
    free_label_table(label_tbl,labels_cnt);
    free_gen_table(entries_table, entries_cnt);
    free_gen_table(externs_table, externs_cnt);
}