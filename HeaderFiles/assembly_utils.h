#ifndef C_PROJECT_OPENU_ASSEMBLY_UTILS_H
#define C_PROJECT_OPENU_ASSEMBLY_UTILS_H

typedef struct opcode {
    char *opcode;  
    int arg_num;     
} opcode;

/* Represents a command */
typedef struct command {
    char *cmd_label;     
    int opcode;      
    char *cmd_src;    
    char *cmd_dst;     
} command;


typedef struct instruction {
    char *inst_label;      
    short *nums;     
    int len;           
    char *arg_label;  
    int is_entry;    
} instruction;

/**
 * This function checks if a given string matches any known instruction names.
 * It compares the provided string against a list of known instruction names and returns a flag indicating
 * whether there is a match.
 * @param str  Pointer to the null-terminated string to be checked.
 * @return 1 if the string matches one of the known instruction names; 0 otherwise.
 */
int name_as_inst(char *str);

/**
 * This function determines the index of a given opcode in the list of known opcodes.
 * It searches for a provided string in a list of known opcodes and returns the index of the matching opcode.
 * If the string does not match any opcode, it returns -1.
 * @param str  Pointer to the null-terminated string representing the opcode to be searched.
 * @return The index of the matching opcode if found; -1 otherwise.
 */
int which_opcode(char *str);

/**
 * This function finds the index of a given register name in the list of known registers.
 * It searches for a provided string in a list of known register names and returns the index of the matching register.
 * If the string does not match any register name, it returns -1.
 * @param str  Pointer to the null-terminated string representing the register name to be searched.
 * @return The index of the matching register if found; -1 otherwise.
 */
int which_reg(char *str);

instruction *create_instruction2(char *buffer, char *error_msg);

/**
 * This function creates and initializes an `instruction` structure from a given buffer.
 * It parses the buffer to extract and initialize an `instruction` structure. The structure is populated with 
 * the label extracted from the buffer and other properties such as `is_entry`. Memory is allocated for the new instruction,
 * and error messages are set if label validation fails.
 * @param buffer  Pointer to a null-terminated string containing the data to parse.
 * @param is_entry  Integer indicating if the instruction is marked as an entry (non-zero for entry, zero otherwise).
 * @param error_msg  Pointer to a buffer for storing error messages if any issues arise.
 * @return A pointer to the newly created `instruction` structure if successful; NULL otherwise.
 * @note The function performs the following steps:
 * - Copies the input buffer to a temporary buffer.
 * - Extracts the label from the buffer.
 * - Validates the extracted label using the `validate_label` function.
 * - Allocates memory for the `instruction` structure and initializes its fields.
 * - Sets `error_msg` if the label validation fails.
 */
instruction *create_instruction(char *buffer, int is_entry,char *error_msg);

/**
 * This function checks if the provided string meets several criteria to be considered a valid label. A label is deemed valid
 * if it satisfies the following conditions:
 * - It is not NULL.
 * - Its length does not exceed a maximum allowed length.
 * - It is not recognized as an opcode or instruction name.
 * - It consists only of alphanumeric characters (letters and digits) after the initial characters.
 * @param label  Pointer to the null-terminated string representing the label to be validated.
 * @return 1 if the label is valid; 0 otherwise.
 */
int validate_label(char *label);

/**
 * This function checks if the provided string meets several criteria to be considered a valid label declaration. A valid
 * label declaration must adhere to the following rules:
 * - It is not NULL and does not exceed a maximum allowed length.
 * - It is not recognized as an opcode, instruction name, or register name.
 * - It starts with an alphabetic character.
 * - It must end with a colon `:` that is immediately preceded by an alphabetic character and immediately followed by the end of the string.
 * @param label  Pointer to the null-terminated string representing the label declaration to be validated.
 * @return 1 if the label declaration is valid; 0 otherwise.
 */
int validate_label_declaration(char *label);

/**
 * This function parses a buffer to extract and build an array of short integers for an instruction.
 * It processes a string buffer containing comma-separated integers (representing instruction data) to
 * populate the `nums` array in the provided `instruction` structure. It also performs validation to ensure the
 * data format is correct and the values fall within acceptable ranges.
 * @param buffer  Pointer to a null-terminated string containing the data to parse. The function expects this buffer
 *                to be positioned correctly in the string.
 * @param inst    Pointer to an `instruction` structure where the parsed data will be stored.
 * @param error_msg  Pointer to a buffer for storing error messages if any issues arise.
 * @return 1 if the array was successfully built and validated; 0 otherwise.
 */
int build_array(char *buffer, instruction *inst, char *error_msg);

int build_string(char *buffer, instruction *inst, char *error_msg);

#endif