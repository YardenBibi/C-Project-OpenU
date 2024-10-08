#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/tables.h"

void insert_general_label(gen_label_table **table, int cnt, instruction *inst, int line, char* name){
    gen_label_table *tbl;
    int label_length;
    char copy[MAX_LABEL_LENGTH];
    strcpy(copy,name);
    tbl = *table;

    (*table + cnt - 1)->line = line;
    label_length = strlen(copy) + 1;
    (*table + cnt - 1)->name = manual_malloc(label_length * sizeof(char));
    if ((*table + cnt - 1)->name == NULL) {
        return ;
    }
    strcpy((*table + cnt - 1)->name, copy);
    *table = realloc(*table, (cnt + 1) * sizeof(gen_label_table));
    if (table == NULL) {
        general_errors("failed to allocate memory");
        free(tbl);
    }

}

int insert_label_table(label_table **label_tbl, int labels_cnt, char *label, int counter, int line, int is_data_line,char *error_msg){
    label_table *temp;
    char copy[MAX_LABEL_LENGTH];
    int i;
    strcpy(copy,label);
    for (i = 0; i < labels_cnt - 1; i++) {
        if (strcmp((*label_tbl + i)->name,label) == 0) {
            sprintf(error_msg, "The label %s is defined twice - in line %d and line %d\n",label,line,(*label_tbl + i)->line);
            return 0;
        }
    }
    
    temp = *label_tbl;
    *label_tbl = realloc(*label_tbl, labels_cnt * sizeof(label_table));
    if (*label_tbl == NULL) {
        strcpy(error_msg, "Failed to allocate memory");
        free(temp);
        return 0;
    }
    
    (*label_tbl + labels_cnt - 1)->is_data_line = is_data_line;
    (*label_tbl + labels_cnt - 1)->address = counter;
    (*label_tbl + labels_cnt - 1)->line = line;
     
    if(label ==NULL){
        (*label_tbl + labels_cnt - 1)->name = NULL;
    }
    else {
        (*label_tbl + labels_cnt - 1)->name = malloc((strlen(label) + 1) * sizeof(char));   
        if ((*label_tbl + labels_cnt - 1)->name == NULL) {
            strcpy(error_msg, "Failed to allocate memory");
            return 0;
        }
        strcpy((*label_tbl + labels_cnt - 1)->name, copy);
    }
    return 1;
}

unsigned short command_to_short(gen_label_table **externs_table,int externs_cnt, gen_label_table **entries_table,int entries_cnt, command *cmd, unsigned short *n_are) {
    unsigned short n_src, n_op, n_dest;
    n_src = n_op = n_dest = 0;
    if (which_reg(cmd->cmd_src) >= 0) {
        if (cmd->cmd_src[0] == '*'){n_src = (short) (4 << 7);}
        else                       {n_src = (short) (8 << 7);}
    }
    else if (validate_label(cmd->cmd_src)) {
        n_src = (short) (2 << 7);  
    }
    
    else if (is_operand_number(cmd->cmd_src)) {
        n_src = (short) (1 << 7);  
    }
    
    if (which_reg(cmd->cmd_dst) >= 0) {
        if (cmd->cmd_dst[0] == '*'){n_dest = (short) (4 << ARE_BITS);}
        else                       {n_dest = (short) (8 << ARE_BITS);}
        *n_are=4;
    }
    else if (validate_label(cmd->cmd_dst)) {
        n_dest = (short) (2 << 3); 
        
    }
         
    else if (is_operand_number(cmd->cmd_dst) && cmd->opcode<=5) {
        n_dest = (short) (1 << 3);  
    }
    
    n_op = (short) (cmd->opcode) << 11; 
    return ((n_op | n_src) | (n_dest | 4));
}

void update_labels(label_table *label_tbl, int lines, int IC) {
    int i;
    for (i = 0; i < lines; i++) {
        if (label_tbl[i].is_data_line) {
            label_tbl[i].address += IC + 1;
        }
    }
    for (i = 0; i < lines; i++) {
        label_tbl[i].address += IC_INIT;
    }
}

void handle_externs_address(converted_line *code_lines, gen_label_table *externs_table, int externs_cnt, int IC_DC) {
    int i, j;
    for (i = 0; i <= IC_DC; i++) {
        if (code_lines[i].label != NULL) {
            for (j = 0; j < externs_cnt; j++) {
                if (strcmp(code_lines[i].label, externs_table[j].name) == 0) {
                    code_lines[i].short_num = 1;
                }
            }
        }
    }
}

void handle_labels(converted_line *code_lines, label_table *label_tbl, int labels_cnt, int IC, char *filename, char *error_msg) {
    int i, j, found;
    for (i = 0; i <= IC; i++) {
        found = 0;
        if (code_lines[i].label != NULL && code_lines[i].short_num == 2) {
            for (j = 0; j < labels_cnt && found == 0; j++) {
                if (strcmp(code_lines[i].label, label_tbl [j].name) == 0) {
                    code_lines[i].short_num |= label_tbl [j].address << ARE_BITS;
                    found = 1;
                }
            }
            if (!found) {
                sprintf(error_msg,"The label %s isn't defined in the given file",code_lines[i].label);
                input_errors(filename,code_lines[i].line_num, error_msg);
            }
        }
    }
}

void create_ext_file(converted_line *code_lines,int IC_DC,gen_label_table *externs_table, int externs_cnt,char *filename){
    char* ext_file, error_buffer[INITIAL_BUFFER_SIZE];
    int i,j,written=0;
    FILE *file;
    ext_file = generate_file_name(filename, ".ext");
    
    file = fopen(ext_file,"w");
    if (file==NULL){
        sprintf(error_buffer,"Couldn't open file %s", ext_file);
        general_errors(error_buffer);
        return ;
    }

    for (i = 0; i <= IC_DC; i++) {
        if (code_lines[i].label != NULL) {
            for (j = 0; j < externs_cnt; j++) {
                if (strcmp(code_lines[i].label, externs_table[j].name) == 0) {
                    fprintf(file, "%s\t%d\n", externs_table[j].name, IC_INIT + i);
                    written = 1; /* Indicates there's content in ent file */
                }
            }
        }
    }
    fclose(file);
    if(!written){remove(ext_file);}
    else{delete_extra_line(ext_file);}
}


void create_ent_file(gen_label_table *entries_table, int entries_cnt,label_table *label_tbl,int labels_cnt, char *filename){
    char* ent_file, error_buffer[INITIAL_BUFFER_SIZE];
    int i,j,written=0;
    FILE *file;
    ent_file = generate_file_name(filename, ".ent");
    
    file = fopen(ent_file,"w");
    if (file==NULL){
        sprintf(error_buffer,"Couldn't open file %s", ent_file);
        general_errors(error_buffer);
        return ;
    }

    for (i = 0; i <= labels_cnt; i++) {
        if (label_tbl[i].name != NULL) {
            for (j = 0; j < entries_cnt; j++) {
                if (strcmp(label_tbl[i].name, entries_table[j].name) == 0) {
                    fprintf(file, "%s\t%d\n", entries_table[j].name, label_tbl[i].address);
                    written = 1; /* Indicates there's content in ent file */
                }
            }
        }
    }
    fclose(file);
    if(!written){remove(ent_file);}
    else{delete_extra_line(ent_file);}
}

void free_label_table(label_table *label_tbl, int labels_cnt){
    int i;
    for (i = 0; i < labels_cnt; i++) {
        free(label_tbl[i].name);
    }
    free(label_tbl);
}

void free_gen_table(gen_label_table *tbl, int cnt) {
    int i;
    for (i = 0; i < cnt; i++) {
        free(tbl[i].name);
    }
    free(tbl);
}