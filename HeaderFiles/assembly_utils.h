#ifndef C_PROJECT_OPENU_ASSEMBLY_UTILS_H
#define C_PROJECT_OPENU_ASSEMBLY_UTILS_H

/* Represents an opcode */
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
    int already_done;   
} command;

/* Represents an instruction */
typedef struct instruction {
    char *inst_label;      
    short *nums;     
    int len;           
    char *arg_label;  
    int is_entry;    
} instruction;

/* Represents a converted command to binary representation */
typedef struct converted_line {
    unsigned short short_num; 
    char *label; 
    int line_num; 
} converted_line;

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

/**
 * This function processes a buffer containing a `.string` declaration in assembly code,
 * extracting the string and storing it in the `instruction` structure. It verifies the
 * proper format of the string, ensuring it starts and ends with double quotes (`"`),
 * and that no extra characters exist after the closing quote.
 * @param buffer The buffer containing the line of code with the `.string` declaration.
 * @param inst Pointer to the `instruction` structure where the processed string will be stored.
 * @param error_msg Pointer to a buffer for storing error messages in case of invalid input.
 * @return 1 if the string is successfully processed and stored, 0 if an error occurs.
 *         In case of an error, an appropriate message is stored in `error_msg`.
 */
int build_string(char *buffer, instruction *inst, char *error_msg);

/**
 * This function adds the machine code instructions to an array of `converted_line` structures.
 * It converts the data stored in the `instruction` structure into machine code, appending it
 * to the array of instruction lines. Each entry in the array represents a single unit of machine code
 * and is associated with the line number from the assembly source.
 * @param inst_lines Pointer to a pointer of `converted_line` structures, which represents
 *                   the array where the machine code instructions will be added.
 * @param inst Pointer to the `instruction` structure containing the machine code data to be added.
 * @param DC Pointer to an integer representing the current index in the `inst_lines` array.
 *           This index is incremented as new instructions are added.
 * @param line Integer representing the line number in the assembly source from which the instruction
 *             was derived. This is stored in each `converted_line` entry.
 * @return void This function does not return a value. It modifies the `inst_lines` array and `DC` index
 *         to include the new machine code instructions.
 */
void add_machine_code_inst(converted_line **inst_lines, instruction *inst, int *DC, int line);

/**
 * This function adds a machine code command to an array of `converted_line` structures. 
 * It stores the provided numerical value (`num`), the associated label (if any), 
 * and the assembly source line number into the `code_lines` array. The array is dynamically 
 * resized to accommodate new entries.
 * @param code_lines Pointer to a pointer of `converted_line` structures, which represents 
 *                   the array where the machine code command will be added.
 * @param num Unsigned short representing the numerical value of the machine code to be added.
 * @param str Pointer to a null-terminated string representing the label associated with the 
 *            machine code command. If no label is associated, this should be `NULL`.
 * @param IC Pointer to an integer representing the current index in the `code_lines` array. 
 *           This index is incremented as new commands are added.
 * @param line Integer representing the line number in the assembly source from which the 
 *             command was derived. This is stored in each `converted_line` entry.
 * @return int Returns 1 if the machine code command was successfully added to the array. 
 *         Returns 0 if memory allocation fails for either the `code_lines` array or the label string.
 */
int add_machine_code_cmd(converted_line **code_lines, unsigned short num, char *str, int *IC, int line);

/**
 * This function increases the allocated memory for an array of `converted_line` structures.
 * It reallocates memory for the array to accommodate an additional entry, ensuring that
 * the array can store more machine code lines. If reallocation fails, the function handles 
 * the error and frees the previously allocated memory.
 * @param lines Pointer to a pointer of `converted_line` structures, which represents the array
 *              for which the memory allocation is to be increased.
 * @param counter Integer representing the current number of entries in the array. The function
 *                uses this value to determine the new size of the array after reallocation.
 * @return int Returns 1 if the memory reallocation was successful, allowing for the array to
 *         accommodate more entries. Returns 0 if memory reallocation fails.
 */
int inc_mem(converted_line **lines, int counter);

/**
 * Parses a buffer containing an assembly command and creates a `command` structure.
 * The function handles label validation, opcode identification, and operand validation,
 * ensuring that the command is properly formatted according to the instruction set.
 * If an error is encountered, an appropriate error message is stored in `error_msg`,
 * and the function returns `NULL`.
 * @param buffer The input string containing the command to be parsed. This string is expected
 *               to be a single line of an assembly instruction with optional labels and arguments.
 * @param error_msg A buffer for storing error messages if any validation fails during command creation.
 * @return A pointer to a newly created `command` structure if the command is successfully parsed.
 *         If an error occurs, `NULL` is returned and an appropriate error message is stored in `error_msg`.
 * This function performs the following operations:
 * - Allocates memory for a `command` structure and initializes it.
 * - Checks for and processes a label if it is present at the beginning of the command.
 * - Identifies the opcode of the command and verifies that it is valid.
 * - Validates the operands based on the identified opcode, ensuring they conform to the expected format.
 * - Handles various cases for different opcodes, ensuring operands are correctly assigned and validated.
 * - Returns the `command` structure if successful, or `NULL` if an error occurs.
 */
command *create_command(char *buffer, char *error_msg);

/**
 * Checks if a given string represents a valid integer number.
 * @param str A pointer to a null-terminated string to be checked.
 *            The string can contain leading or trailing spaces, but they are ignored during conversion.
 * @return 1 if the string represents a valid integer number, otherwise 0.
 */
int is_number(char *str);

/**
 * Checks if a given string represents a valid operand number in the format `#<number>`.
 * The function verifies that the string starts with a `#` character followed by a valid integer.
 * @param str A pointer to a null-terminated string to be checked.
 *            The string must start with `#` followed by an integer number to be considered valid.
 * @return 1 if the string represents a valid operand number in the format `#<number>`, otherwise 0.
 */
int is_operand_number(char *str);

/**
 * Determines if a given string represents a valid operand in an assembly instruction.
 * The function checks if the string is either a register, a label, or an operand number in the format `#<number>`.
 * @param arg A pointer to the null-terminated string representing the operand to be validated.
 * @return 1 if the string is a valid operand; otherwise, 0.
 */
int is_legal_operand(char *arg);

/**
 * Determines if a given string represents a valid single operand in an assembly instruction.
 * This function checks whether the operand is a register (excluding registers prefixed with `*`),
 * a label, or a number operand in the format `#<number>`.
 * @param arg A pointer to the null-terminated string representing the operand to be validated.
 * @return 1 if the string is a valid single operand; otherwise, 0.
 */
int is_legal_single_operand(char *arg);

/**
 * Converts register operands from a command structure into a binary representation.
 * The function processes either the source or destination register operand, converting
 * it into a binary value suitable for use in machine code instructions.
 * @param cmd A pointer to the `command` structure containing the operands.
 * @param is_src An integer flag indicating whether the source register should be processed.
 *               - If `is_src` is non-zero, the function processes the source register.
 *               - If `is_src` is zero, the function processes the destination register.
 * @return The binary representation of the register(s) as an unsigned short.
 * or returns a special value of `65535` if the destination register was already
 * processed or is invalid in the context.
 */
unsigned short convert_reg_to_binary(command *cmd, int is_src);

/**
 * Adds an information line to the machine code based on the provided command and argument.
 * The function determines whether the argument is a register, label, or immediate value, 
 * and appropriately encodes it as machine code.
 * @param code_lines A double pointer to the array of `converted_line` structures, which stores
 *                   the converted assembly code lines.
 * @param cmd A pointer to the `command` structure containing the source and destination operands.
 * @param IC A pointer to the Instruction Counter, which is incremented as new lines are added.
 * @param is_src An integer flag indicating whether the source operand is being processed.
 * @param line The line number in the assembly source code, used for associating the machine
 *             code with the corresponding source line.
 * @param n_are An unsigned short representing the ARE (Absolute, Relocatable, External) bits
 *              for the instruction, to be combined with the numerical value of the operand.
 * @return An integer value indicating success or failure:
 *         - Returns `1` if the information line was added successfully.
 *         - Returns `0` if an error occurred, such as memory allocation failure.
 */
int add_information_line(converted_line **code_lines, command *cmd, int *IC, int is_src, int line, unsigned short n_are);

/**
 * Merges the data section into the machine code section, placing the data lines at the 
 * end of the command code lines.
 * @param code_lines A double pointer to the array of `converted_line` structures, which stores
 *                   the converted assembly code lines (both command and data).
 * @param data A pointer to the array of `converted_line` structures containing the data lines
 *             to be appended to the `code_lines` array.
 * @param IC The instruction counter, representing the number of command lines in `code_lines`.
 * @param DC The data counter, representing the number of data lines in the `data` array.
 * @return An integer value indicating success or failure:
 *         - Returns `1` if the merge operation was successful.
 *         - Returns `0` if memory allocation for the combined array failed.
 */
int merge_all_lines(converted_line **code_lines, converted_line *data, int IC, int DC);

/**
 * Creates an object file with the machine code representation of the program.
 * The object file contains the machine code for both instruction and data sections.
 * @param code_lines A pointer to the array of `converted_line` structures, which holds the 
 *                   machine code for both instruction and data lines.
 * @param IC The instruction counter, representing the number of command lines in `code_lines`.
 * @param DC The data counter, representing the number of data lines in `code_lines`.
 * @param filename The name of the source file, used to generate the output object file name.
 */
void create_ob_file(converted_line *code_lines, int IC, int DC, char *filename);

/**
 * Frees the memory allocated for the machine code lines and their associated labels.
 * This function iterates over the array of `converted_line` structures, and freeing the memory.
 * @param code_lines A pointer to the array of `converted_line` structures, which holds the 
 *                   machine code and optional labels.
 * @param IC_DC The total number of lines in `code_lines`, including both instruction and data lines.
 */
void free_machine_code(converted_line *code_lines, int IC_DC);

#endif