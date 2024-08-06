#ifndef C_PROJECT_OPENU_TABLES_H
#define C_PROJECT_OPENU_TABLES_H

#include "assembly_utils.h"

typedef struct label_table {
    char *name;
    int address;
    int line;
    int is_data_line;
} label_table;


/**
 * Represents other labels (non-instruction labels) and their assembly line information.
 *
 * This struct represents information about other labels (non-instruction labels) in the assembly code.
 * It includes the name of the label and the assembly line number where the label is used.
 */
typedef struct gen_label_table {
    char *name;
    int line;
} gen_label_table;

/**
 * This function inserts a general label entry in a label table.
 * It updates the last entry in the `gen_label_table` with information from the provided `instruction` structure.
 * It resizes the table to accommodate a new entry and copies the label name from the instruction.
 * @param table  Pointer to a pointer to the `gen_label_table` array. This is updated to include the new entry.
 * @param cnt    The current count of entries in the table before insertion.
 * @param inst   Pointer to an `instruction` structure containing the label information to be inserted.
 * @param line   The line number where the label was found in the source code.
 */
void insert_general_label(gen_label_table **table, int cnt, instruction *inst, int line);


#endif