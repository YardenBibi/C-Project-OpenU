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
