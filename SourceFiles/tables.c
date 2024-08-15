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
    printf("INSERTING ENTRY: %s\n",copy);
    tbl = *table;

    /* Update the assembly_line field of the last entry in the table */
    (*table + cnt - 1)->line = line;

    /* Calculate the length of the label_name */
    label_length = strlen(copy) + 1;

    /* Allocate memory for the label_name and copy the label from 'inst' */
    (*table + cnt - 1)->name = manual_malloc(label_length * sizeof(char));
    if ((*table + cnt - 1)->name == NULL) {
        return ;
    }
    /*Copy the data into the table*/
    strcpy((*table + cnt - 1)->name, copy);

    /* Resize the table to accommodate the new entry */
    *table = realloc(*table, (cnt + 1) * sizeof(gen_label_table));
    if (table == NULL) {
        /*Failed to allocate memory*/
        general_errors("failed to allocate memory");
        free(tbl);
    }

}

int insert_label_table(label_table **label_tbl, int labels_cnt, char *label, int counter, int line, int is_data_line,char *error_msg){
    label_table *temp;
    char copy[MAX_LABEL_LENGTH];
    int i;
    strcpy(copy,label);
    printf("INSERTING LABEL:%s\n",label);
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
    printf("INSERTED LABEL:%s\n",label);
    return 1;
}

unsigned short command_to_short(gen_label_table **externs_table,int externs_cnt, gen_label_table **entries_table,int entries_cnt, command *cmd, unsigned short *n_are) {
    unsigned short n_src, n_op, n_dest;
    int extern_match=0,entry_match=0, i,flag=0;
    n_src = n_op = n_dest = 0;
    printf("N_ARE:%d\n",*n_are);
    printf("SRC:%s\n",cmd->cmd_src);
    printf("RESULT:%d\n",which_reg(cmd->cmd_src));
    printf("DST:%s\n",cmd->cmd_dst);
    printf("RESULT:%d\n",which_reg(cmd->cmd_dst));
    printf("EXTERNS CNT:%d, ENTRIES CNT:%d\n",externs_cnt,entries_cnt);
    if (entries_cnt==1){
        printf("ENTRY:%s\n",(entries_table[0]->name));
    }
    if (which_reg(cmd->cmd_src) >= 0) {
        flag=1;
        if (cmd->cmd_src[0] == '*'){n_src = (short) (4 << 7);}
        else                       {n_src = (short) (8 << 7);}
    }
    else if (validate_label(cmd->cmd_src)) {
        n_src = (short) (2 << 7);  
        for (i = 0; i < externs_cnt; i++) {
            if (externs_table[i] != NULL && externs_table[i]->name != NULL && strcmp(externs_table[i]->name, cmd->cmd_src) == 0) {
            extern_match = 1; 
            }
        }
        for (i = 0; i < entries_cnt; i++) {
            if (entries_table[i] != NULL && entries_table[i]->name != NULL && strcmp(entries_table[i]->name, cmd->cmd_src) == 0) {
            entry_match = 1; 
            }
        }
        if (extern_match){*n_are=1;}
        else if (entry_match){*n_are=2;}
    }
    
    else if (is_operand_number(cmd->cmd_src)) {
        n_src = (short) (1 << 7);  
    }
    
    if (which_reg(cmd->cmd_dst) >= 0) {
        if (cmd->cmd_dst[0] == '*'){n_dest = (short) (4 << 3);}
        else                       {n_dest = (short) (8 << 3);}
        *n_are=4;
    }
    else if (validate_label(cmd->cmd_dst)) {
        n_dest = (short) (2 << 3); 
        for (i = 0; i < externs_cnt; i++) {
            if (externs_table[i] != NULL && externs_table[i]->name != NULL && strcmp(externs_table[i]->name, cmd->cmd_dst) == 0) {
            extern_match = 1; 
            }
        }
        for (i = 0; i < entries_cnt; i++) {
            if (entries_table[i] != NULL && entries_table[i]->name != NULL && strcmp(entries_table[i]->name, cmd->cmd_dst) == 0) {
            entry_match = 1; 
            }

        }
        if (!flag){
            if (extern_match){*n_are=1;}
            else if (entry_match){*n_are=2;}
        }
    }
         
    else if (is_operand_number(cmd->cmd_dst) && cmd->opcode<=5) {
        n_dest = (short) (1 << 3);  
    }
    
    n_op = (short) (cmd->opcode) << 11; 
    printf("N_ARE:%d\n",*n_are);
    printf("N_DESR:%d\n",n_dest);
    return ((n_op | n_src) | (n_dest | 4));
}

void update_labels(label_table *label_tbl, int lines, int IC) {
    int i;
    for (i = 0; i < lines; i++) {
        if ((label_tbl + i)->is_data_line) {
            (label_tbl + i)->address += IC + 1;
        }
    }
    for (i = 0; i < lines; i++) {
        (label_tbl + i)->address += IC_INIT;
        printf("LABEL %s ADDRESS:%d\n",(label_tbl + i)->name,(label_tbl + i)->address);
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
        if ((code_lines + i)->label != NULL && (code_lines + i)->short_num == 2) {
            printf("SEARCHING %s\n",(code_lines + i)->label);
            for (j = 0; j < labels_cnt && found == 0; j++) {
                if (strcmp((code_lines + i)->label, (label_tbl + j)->name) == 0) {
                    (code_lines + i)->short_num |= (label_tbl + j)->address << ARE_BITS;
                    printf("FOUND %s, ADDRESS:%d\n",(code_lines + i)->label,(code_lines + i)->short_num);
                    found = 1;
                }
            }
            if (!found) {
                sprintf(error_msg,"The label %s isn't defined in the given file",(code_lines + i)->label);
                input_errors(filename,(code_lines + i)->line_num, error_msg);
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
                    written = 1;
                }
            }
        }
    }
    fclose(file);
    if(!written){remove(ext_file);}
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
                    written = 1;
                }
            }
        }
    }
    fclose(file);
    if(!written){remove(ent_file);}
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