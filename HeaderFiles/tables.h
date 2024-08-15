#ifndef C_PROJECT_OPENU_TABLES_H
#define C_PROJECT_OPENU_TABLES_H

#include "assembly_utils.h"

typedef struct label_table {
    char *name;
    int address;
    int line;
    int is_data_line;
} label_table;


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
void insert_general_label(gen_label_table **table, int cnt, instruction *inst, int line, char* name);

/**
 * Inserts a new label into the label table, ensuring that there are no duplicate labels. 
 * This function allocates or reallocates memory for the label table, updates the label's properties,
 * and checks for duplicate labels. If a duplicate is found, an error message is generated.
 * @param label_tbl A pointer to a pointer to the label table array. This array will be reallocated 
 *                  if necessary to accommodate the new label.
 * @param labels_cnt The current number of labels in the table before the insertion.
 * @param label The label to be inserted. If this is `NULL`, the label name will be set to `NULL`.
 * @param counter The address associated with the label.
 * @param line The line number where the label was defined.
 * @param is_data_line A flag indicating whether the label is associated with data (1) or code (0).
 * @param error_msg A buffer to store any error messages generated during the process.
 * @return 1 if the label was successfully inserted, or 0 if an error occurred. 
 */
int insert_label_table(label_table **label_tbl, int labels_cnt, char *label, int counter, int line, int is_data_line,char *error_msg);

/**
 * Converts a `command` structure into a binary short representation used for machine code.
 * The function processes the source and destination operands, determines their addressing modes,
 * and integrates label information from the externs and entries tables. It combines these 
 * components into an unsigned short value, including the opcode and addressing mode bits.
 * @param externs_table A pointer to the array of external labels. This is used to determine 
 *                      if the source or destination labels are external.
 * @param externs_cnt The number of external labels in the `externs_table`.
 * @param entries_table A pointer to the array of entry labels. This is used to determine 
 *                      if the source or destination labels are entry labels.
 * @param entries_cnt The number of entry labels in the `entries_table`.
 * @param cmd A pointer to the `command` structure containing the opcode, source, and destination operands.
 * @param n_are A pointer to a short that will be updated with the addressing mode (ARE) for the operands.
 * 
 * @return An unsigned short value representing the binary machine code for the given command. 
 *         This value includes the opcode and the addressing modes for the source and destination operands.
 * 
 * This function performs the following tasks:
 * - Determines the addressing mode for the source and destination operands.
 * - Checks if the operands are registers, labels, or numerical values and sets the appropriate bits.
 * - Combines the opcode, source addressing bits, and destination addressing bits into a final binary representation.
 */
unsigned short command_to_short(gen_label_table **externs_table,int externs_cnt,gen_label_table **entries_table,int entries_cnt,  command *cmd, unsigned short *n_are);

/**
 * Updates the addresses in the label table based on whether labels are associated with 
 * data lines or code lines. The function adjusts the addresses by adding the current 
 * instruction counter (IC) value and a fixed initial value (IC_INIT) to compute the final 
 * addresses for both code and data labels.
 * @param label_tbl A pointer to the label table, an array of `label_table` structures.
 * @param lines The number of labels in the label table.
 * @param IC The current instruction counter value, used to adjust addresses for data labels.
 * This function performs the following tasks:
 * - For labels associated with data lines (`is_data_line` is true), it increments the address 
 *   by the current instruction counter (IC) plus 1. This adjustment accounts for the position 
 *   of data labels relative to code labels.
 * - It then increments all label addresses by a fixed initial value (IC_INIT) to set the final 
 *   address.
 */
void update_labels(label_table *label_tbl, int lines, int IC) ;

/**
 * Updates the addresses in the `code_lines` array for entries that have labels matching
 * those in the `externs_table`. The function sets the value of `short_num` to indicate
 * that the label is external.
 * @param code_lines Pointer to an array of `converted_line` structures representing the code lines.
 * @param externs_table Pointer to an array of `gen_label_table` structures representing the external labels.
 * @param externs_cnt The number of external labels in the `externs_table`.
 * @param IC_DC The total count of code and data lines, used to iterate through the `code_lines`.
 */
void handle_externs_address(converted_line *code_lines, gen_label_table *externs_table, int externs_cnt, int IC_DC);

/**
 * Updates the addresses in the `code_lines` array for entries that have labels matching
 * those in the `label_tbl`. The function replaces the `short_num` field with the address
 * of the label from the `label_tbl`, adjusting for addressing bits. If a label is not found
 * in the `label_tbl`, it reports an error.
 * @param code_lines Pointer to an array of `converted_line` structures representing the code lines.
 * @param label_tbl Pointer to an array of `label_table` structures representing the labels defined in the assembly.
 * @param labels_cnt The number of labels in the `label_tbl`.
 * @param IC The instruction counter, used to iterate through the `code_lines`.
 * @param filename The name of the assembly file, used for error reporting.
 * @param error_msg Buffer for storing error messages.
 */

void handle_labels(converted_line *code_lines, label_table *label_tbl, int labels_cnt, int IC, char *filename, char *error_msg);

/**
 * This function creates an `.ext` file listing external labels and their addresses from the `code_lines` array.
 * External labels are labels that are declared as extern in the assembly file. The function
 * writes these labels and their addresses to the file.
 * @param code_lines Pointer to an array of `converted_line` structures representing the code lines.
 * @param IC_DC The sum of the instruction counter (IC) and data counter (DC), used to iterate through the `code_lines`.
 * @param externs_table Pointer to an array of `gen_label_table` structures representing external labels.
 * @param externs_cnt The number of external labels in the `externs_table`.
 * @param filename The name of the assembly file, used to generate the `.ext` file name.
 */
void create_ext_file(converted_line *code_lines,int IC_DC, gen_label_table *externs_table, int externs_cnt,char *filename);

/**
 * Creates an `.ent` file listing entry labels and their addresses from the `entries_table` and `label_tbl`.
 * Entry labels are labels that are marked as entries in the assembly file. The function writes these labels
 * and their addresses to the file.
 * @param entries_table Pointer to an array of `gen_label_table` structures representing entry labels.
 * @param entries_cnt The number of entry labels in the `entries_table`.
 * @param label_tbl Pointer to an array of `label_table` structures representing all labels.
 * @param labels_cnt The number of labels in the `label_tbl`.
 * @param filename The name of the assembly file, used to generate the `.ent` file name.
 */
void create_ent_file(gen_label_table *entries_table, int entries_cnt,label_table *label_tbl,int labels_cnt, char *filename);

/**
 * Frees the memory allocated for the `label_tbl` array and its elements.
 * Each label entry in the `label_tbl` has a dynamically allocated `name` field,
 * which must be freed individually. After freeing the `name` fields, the function
 * frees the entire `label_tbl` array itself.
 * @param label_tbl Pointer to an array of `label_table` structures.
 * @param labels_cnt The number of labels in the `label_tbl` array.
 */
void free_label_table(label_table *label_tbl, int labels_cnt);

/**
 * Frees the memory allocated for the `tbl` array and its elements.
 * Each entry in the `tbl` array has a dynamically allocated `name` field,
 * which must be freed individually. After freeing the `name` fields, the function
 * frees the entire `tbl` array itself.
 * @param tbl Pointer to an array of `gen_label_table` structures.
 * @param cnt The number of entries in the `tbl` array.
 */
void free_gen_table(gen_label_table *tbl, int cnt);

#endif