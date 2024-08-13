#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/tables.h"

void insert_general_label(gen_label_table **table, int cnt, instruction *inst, int line){
    gen_label_table *tbl;
    int label_length;
    tbl = *table;

    /* Update the assembly_line field of the last entry in the table */
    (*table + cnt - 1)->line = line;

    /* Calculate the length of the label_name */
    label_length = strlen(inst->arg_label) + 1;

    /* Allocate memory for the label_name and copy the label from 'inst' */
    (*table + cnt - 1)->name = manual_malloc(label_length * sizeof(char));
    if ((*table + cnt - 1)->name == NULL) {
        return ;
    }
    /*Copy the data into the table*/
    strcpy((*table + cnt - 1)->name, inst->arg_label);

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
    (*label_tbl + labels_cnt - 1)->name = malloc((strlen(label) + 1) * sizeof(char));
    if ((*label_tbl + labels_cnt - 1)->name == NULL) {
        strcpy(error_msg, "Failed to allocate memory");
        return 0;
    }
    strcpy((*label_tbl + labels_cnt - 1)->name, label);
    return 1;
}