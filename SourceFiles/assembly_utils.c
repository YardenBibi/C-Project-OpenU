#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/tables.h"

/* Define the opcodes */
opcode OPCODES[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"lea",  2},
        {"clr",  1},
        {"not",  1},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"red",  1},
        {"prn",  1},
        {"jsr",  1},
        {"rts",  0},
        {"stop", 0}
};

char *INSTUCTIONS[] = {".data", ".string", ".entry", ".extern"};
char *REGS[] = {"*r0", "r0", "*r1", "r1", "*r2", "r2", "*r3", "r3", "*r4", "r4", \
                "*r5", "r5", "*r6", "r6", "*r7", "r7"}; 

int name_as_inst(char *str) {
    int i;
    for (i = 0; i < INSTR_CNT; i++) {
        if (strcmp(str, INSTUCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0; 
}

int which_opcode(char *str) {
    int i;

    for (i = 0; i < OPCODES_CNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return i; 
        }
    }
    return -1; 
}


int which_reg(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }

    for (i = 0; i < REG_CNT*2; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i/2; 
        }
    }
    return -1; 
}

int validate_label(char *label){
    if (label == NULL || (strlen(label) > MAX_LABEL_LENGTH) || \
        (which_opcode(label) >= 0) || (name_as_inst(label))){
            return 0;
    }
    while (*(++label) != '\0' && *(label) != ' ') { 
        if (!(isalpha(*label)) || !(isdigit(*label))){
            return 0;
        }
    }
    return 1;
}

int validate_label_declaration(char *label){
    if (label == NULL || (strlen(label) > MAX_LABEL_LENGTH) || \
        (which_opcode(label) >= 0) || (name_as_inst(label)) || \
        (which_reg(label>=0)) || !(isalpha(*label))){
            return 0;
    }
    while (*(++label) != '\0' && (isalpha(*label) || isdigit(*label))) { ;
        } /*check that : comes rigt after char*/
        if (*(label) == ':'){
            if (*(label - 1) == ' '){return 0;}
            if (*(label + 1) == '\0') {
            *label = '\0';
            return 1;
            }
        }
        return 0;
        
}

instruction *create_instruction(char *buffer, int is_entry, char *error_msg){
    instruction *inst;
    char *buffer_copy,*label,*ptr;
    strcpy(buffer_copy,buffer);
    ptr = strchr(buffer_copy, '.');
    label = strtok(ptr, " ");
    ptr = strchr(label, ' ');
    ptr++; 
    label = strtok(NULL, " \n");

    inst = manual_malloc(sizeof(instruction));
    if (inst == NULL) {
        return NULL;
    }

    if (!validate_label(label)){
        strcpy(error_msg, "Illegal label");
        return NULL;
    }

    inst->arg_label = label;
    inst->is_entry = is_entry;
    inst->nums = NULL;
    inst->len = 0;

    return inst;
}

instruction *create_instruction2(char *buffer, char *error_msg){
    instruction *inst;
    char *buffer_copy,*label,*colon_pos;
    strcpy(buffer_copy,buffer);

    inst = manual_malloc(sizeof(instruction));
    if (inst == NULL) {
        return NULL;
    }

    colon_pos = strchr(buffer_copy, ':');  
    if (colon_pos != NULL) {
        if (*(colon_pos + 1) != ' ' && *(colon_pos + 1) != '\0') {
            memmove(colon_pos + 2, colon_pos + 1, strlen(colon_pos + 1) + 1);  // Shift the rest of the string right by 1
            *(colon_pos + 1) = ' ';  
        }
        label = strtok(buffer_copy, " \n");
        if (!validate_label_declaration(label)) {
            strcpy(error_msg, "Illegal label declaration");
            return NULL;
        }
        inst->inst_label = label;
        label = strtok(NULL, " \n");
    }
    else {
        inst->inst_label = NULL;
        label = strtok(buffer_copy, " \n");
    }

    if (strcmp(label, ".data") == 0) {
        if(!build_array(label,inst, error_msg)){
            return NULL;
        }
    } 
    else if (strcmp(label, ".string") == 0) {
        build_string(label,inst, error_msg);
    }
}

int build_array(char *buffer, instruction *inst, char *error_msg){
    char *array_str, *num;
    int arr_len=1, number, digitFlag = 0, commaFlag = 0, minusFlag = 0, i;

    array_str = strtok(NULL, " \n");
    if (!isdigit(array_str[0]) || array_str[0] != '-'){return 0;}
    for (i = 0; array_str[i]; i++) {
        if(array_str[i] == ' '){
                continue;
        }
        if (array_str[i] == ',') {
            if (!digitFlag || commaFlag) {
                strcpy(error_msg, "Illegal .data instruction");
                return 0; 
            }
            arr_len++;
            commaFlag = 1;
            minusFlag=0;
            digitFlag = 0;
        }
        else if (isdigit(array_str[i]) || array_str[i] == '-') {         
            commaFlag = 0;
            if (array_str[i] == '-') {
                if (minusFlag || digitFlag ){return 0;}
                minusFlag = 1;
                digitFlag = 0;
                continue;
            }
            if (isdigit(array_str[i])) {
                digitFlag = 1;
                minusFlag=0;
            }
        } else {
            strcpy(error_msg, "Illegal .data instruction");
            return 0; 
        }
    }

    if (commaFlag) {return 0;}
    inst->len = arr_len;
    inst->nums = (short *)malloc(inst->len * sizeof(short));
    strtok(array_str, "\n");
    i=0;
    while ((num = strtok(NULL, ", \n")) != NULL) {
        number = (short) (atoi(num));
        if (number > MAX_NUM || number < MIN_NUM) {
            strcpy(error_msg, "Illegal .data instruction - a number is too big");
            return 0;
        } 
        inst->nums[i++]= (short) (atoi(num));
    }
    
    return 1; 
}

int build_string(char *buffer, instruction *inst, char *error_msg){
    char *string;
    int second_pos = -1, str_len, i;

    string = strtok(NULL, " \n");
    if (string[0] != '"') {
        strcpy(error_msg, "Illegal .string declaration - no opening quote");
        return 0;
    }
    for (int i = 1; string[i] != '\0'; i++) { 
        if (string[i] == '"') {
            second_pos = i;
            break;
        }
    }
    if (string[second_pos+1] != '\0' || string[second_pos+1] != '\n') {
        strcpy(error_msg, "Illegal .string declaration - extra chars after string declaration");
        return 0;
    }
    if (second_pos != -1) {
        str_len = second_pos - 1; 
    } else {
        strcpy(error_msg, "Illegal .string declaration - no closing quote");
        return 0; 
    }
    inst->len = str_len + 1;
    inst->nums = (short *)malloc(inst->len * sizeof(short));
    for (int i = 1; i < inst->len; i++) { 
        inst->nums[i - 1] = (short)string[i];
    }
    inst->nums[str_len] = 0;
    return 1;
}

int coma_more_than_one(char *str) {
    int cnt= 0 ;
    char *copy;
    copy = str;

    while (*copy) {
        if (*copy == ","){
            cnt++;
            if(cnt>1){return 1;}
        }
        copy++;
    }
    return 0;
}

int is_number(char *str) {
    char *conv_ptr;
    if (str != NULL) {
        strtol(str, &conv_ptr, 10);
        if (*conv_ptr == '\0' || *conv_ptr == ' ') {
            return 1; 
        }
    }
    return 0; 
}

int is_operand_number(char *str) {
    char *conv_ptr;
    if (str != NULL && str[0] == '#') {
        str++;
        if (*str != '\0') {
            strtol(str, &conv_ptr, 10);
            if (*conv_ptr == '\0' || *conv_ptr == ' ') {
                return 1; 
            }
        }
    }
    return 0; 
}


int is_legal_operand(char *arg){
    return ((which_reg(arg) >= 0) || validate_label(arg) || is_operand_number(arg));
}

int is_legal_single_operand(char *arg){
    return ((which_reg(arg) >= 0 && arg[0]!="*") || validate_label(arg) || is_first_operand_number(arg));
}

void add_machine_code_inst(converted_line **inst_lines, instruction *inst, int *DC, int line){
    int i;
    /*Check if data pointer is NULL before proceeding*/
    /*
    if (inst_lines == NULL || *inst_lines == NULL) {
        return 0;
    }
    */
    i = 0;
    for (i = 0; i < inst->len; i++) {
        /* Check if memory allocation for the code_conv array succeeded */
        if (inc_mem(inst_lines, *DC) == 0) {
            return ;
        }
        /* Set the numerical value in the current code_conv entry */
        (*inst_lines + *DC)->short_num = *(inst->nums + i);

        /* a data line cannot include a label as an ARGUMENT */
        (*inst_lines + *DC)->label = NULL;

        /* Set the assembly line number associated with the code_conv */
        (*inst_lines + *DC)->line_num = line;
        (*DC)++;
    }
}

int add_machine_code_cmd(converted_line **code_lines, unsigned short num, char *str, int *IC, int line) {
    /* Check if memory allocation increase for the code_conv array succeeded */
    if (inc_mem(code_lines, *IC) == 0) {
        return 0;  /* Return 0 if memory allocation fails */
    }

    /* Set the numerical value of the assembly line */
    (*code_lines + *IC)->short_num = num;

    /* Set the assembly line number */
    (*code_lines + *IC)->line_num = line;

    /* Check if a label is available in this line, and handle memory allocation for it. */
    if (str == NULL) {
        (*code_lines + *IC)->label = NULL;  /* No label provided; set label to NULL. */
    } else {
        (*code_lines + *IC)->label = manual_malloc((strlen(str) + 1) * sizeof(char));
        if ((*code_lines + *IC)->label == NULL) {
            return 0;  /* Return 0 if memory allocation for the label fails. */
        }
        strcpy((*code_lines + *IC)->label, str);  /* Copy the label string to the allocated memory. */
    }

    return 1;  /* Return 1 to indicate successful addition of the machine code line. */
}

int inc_mem(converted_line **lines, int counter) {
    converted_line *ptr;
    ptr = *lines;
    /* increasing memory of code for a new word */
    *lines = realloc(*lines, (counter + 1) * sizeof(converted_line));
    if (*lines == NULL) {
        general_errors("Failed to allocate memory");
        free(ptr);
        return 0;
    }
    return 1;
}

command *create_command(char *buffer, char *error_msg){
    char *buffer_copy, *colon_pos, *label, *cmd_part, *arg1, *arg2;
    int flag_visited = 0;

    /* Allocate memory for the command_parts structure */
    command *cmd = manual_malloc(sizeof(command));
    if (cmd == NULL) {
        return NULL;
    }
    cmd_part = strtok(buffer_copy, " \n");
    colon_pos = strchr(buffer_copy, ':');
    if (colon_pos != NULL) {
        if (*(colon_pos + 1) != ' ' && *(colon_pos + 1) != '\0') {
            memmove(colon_pos + 2, colon_pos + 1, strlen(colon_pos + 1) + 1);  
            *(colon_pos + 1) = ' ';  
        }
        if (!validate_label_declaration(label)) {
            strcpy(error_msg, "Illegal label declaration");
            free(cmd);
            return NULL;
        }
        cmd->cmd_label = label;
        cmd_part = strtok(NULL, " \n");
    }
    cmd->opcode = which_opcode(cmd_part);
    if (cmd->opcode == -1){
        free(cmd);
        return NULL;
    }
    cmd_part = strtok(NULL, " \n");
    if (OPCODES[cmd->opcode].arg_num == 0) {
        if (cmd_part != NULL){
            strcpy(error_msg, "Extra text after opcode witout arguments");
            free(cmd);
            return NULL;
        }
        cmd->cmd_src = NULL;
        cmd->cmd_dst = NULL;
        return cmd;
    }
    if (cmd_part == NULL){
        sprintf(error_msg, "Arguments are misssing for the opcode %s",cmd->opcode );
        free(cmd);
        return NULL;
    }
    if (OPCODES[cmd->opcode].arg_num == 1) {
        if (strchr(cmd_part, ' ')) {
            strcpy(error_msg, "Extra text after arguments");
            free(cmd);
            return NULL;
        }
    }
    if (OPCODES[cmd->opcode].arg_num == 2) {
        if (strstr(cmd_part, ",") == NULL) {
            sprintf(error_msg, "Missing comma for opcode %s with two arguents",cmd->opcode);
            free(cmd);
            return NULL;
        } 
        if (coma_more_than_one(cmd_part)) {
            sprintf(error_msg, "Extra comma in opcode %s with two arguents",cmd->opcode);
            free(cmd);
            return NULL;
        }
        arg1 = strtok(cmd_part, ",");
        if (strchr(arg1, ' ')) {
            /*HANDLE IF THERES A TRAILING SPACE*/
        }
        if (!is_legal_operand(arg1)) {
            sprintf(error_msg, "The first operand %s isn't legal",arg1);
            free(cmd);
            return NULL;
        }
        arg2 = strtok(NULL, " \n");
        if (strtok(NULL, "\n")) {
            strcpy(error_msg, "Extra text after arguments");
            free(cmd);
            return NULL;
        }
        if (!is_legal_operand(arg2)) {
            sprintf(error_msg, "The operand %s isn't legal",arg2);
            free(cmd);
            return NULL;
        }   
    }

    switch (cmd->opcode){
        case 1:{
            cmd->cmd_src = arg1;
            cmd->cmd_dst = arg2;
            break;
        }
        case 0:
        case 2:
        case 3:{
            if ((which_reg(arg2) >= 0) || validate_label(arg2)) {
                cmd->cmd_src = arg1;
                cmd->cmd_dst = arg2;
                break;
            } else {
                sprintf(error_msg, "In the given opcode %s - The operand %s \
                isn't label or register", cmd->opcode, arg2);
                free(cmd);
                return NULL;
            }
            break;
        }
        case 4:{
            if (!validate_label(arg1)){
                sprintf(error_msg, "In the given opcode %s - The operand %s isn't a label", cmd->opcode, arg2);
                free(cmd);
                return NULL;
            }
            if (!((which_reg(arg2) >= 0) || validate_label(arg2))) {
                sprintf(error_msg, "In the given opcode %s - The operand %s \
                isn't label or register", cmd->opcode, arg2);
                free(cmd);
                return NULL;
            } 
            cmd->cmd_src = arg1;
            cmd->cmd_dst = arg2;
            break;
        }
        case 12: {
            if (is_legal_single_operand(cmd_part)) {
                cmd->cmd_src = NULL;
                cmd->cmd_dst = cmd_part;
            } else {
                sprintf(error_msg, "In the given opcode %s - The operand %s \
                isn't valid", cmd->opcode, cmd_part);
                free(cmd);
                return NULL;
            }
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13: {
            if ((which_reg(cmd_part) >= 0 && cmd_part[0] != "*") || validate_label(cmd_part)) {
                cmd->cmd_src = NULL;
                cmd->cmd_dst = cmd_part;
            } else {
                sprintf(error_msg, "In the given opcode %s - The operand %s \
                isn't valid", cmd->opcode, cmd_part);           
                free(cmd);
                return NULL;
            }
            break;
        }

        default :{
            sprintf(error_msg, "The given opcode %s isn't valid", cmd->opcode);
            free(cmd);
            return NULL;
        }
    }

    return cmd;
}

unsigned short command_to_short(gen_label_table **externs_table,int externs_cnt, command *cmd) {
    unsigned short n_src, n_op, n_dest, n_are;
    int extern_match=0;
    n_src = n_op = n_dest = 0;
    n_are = 4;

    if (which_reg(cmd->cmd_src) >= 0) {
        if (cmd->cmd_src[0] == "*"){n_src = (short) (4 << 7);}
        else                       {n_src = (short) (8 << 7);}
    }
    else if (validate_label(cmd->cmd_src)) {
        n_src = (short) (2 << 7);  /* Fill in label addressing bits. */
        for (int i = 0; i < externs_cnt; i++) {
            if (externs_table[i] != NULL && externs_table[i]->name != NULL && strcmp(externs_table[i]->name, cmd->cmd_src) == 0) {
            extern_match = 1; 
            }
        }
        if (extern_match){n_are=1;}
        else{n_are=2;}
    }
        /* Check if the source is a numerical value and set the corresponding bits. */
    else if (is_first_operand_number(cmd->cmd_src)) {
        n_src = (short) (1 << 7);  /* Fill in direct addressing bits. */
    }

    if (which_reg(cmd->cmd_dst) >= 0) {
        if (cmd->cmd_dst[0] == "*"){n_dest = (short) (4 << 3);}
        else                       {n_dest = (short) (8 << 3);}
    }
    else if (validate_label(cmd->cmd_dst)) {
        n_dest = (short) (2 << 3);  /* Fill in label addressing bits. */
        for (int i = 0; i < externs_cnt; i++) {
            if (externs_table[i] != NULL && externs_table[i]->name != NULL && strcmp(externs_table[i]->name, cmd->cmd_dst) == 0) {
            extern_match = 1; 
            }
        }
        if (extern_match){n_are=1;}
        else{n_are=2;}
    }
        /* Check if the source is a numerical value and set the corresponding bits. */
    else if (is_first_operand_number(cmd->cmd_dst)) {
        n_dest = (short) (1 << 3);  /* Fill in direct addressing bits. */
    }

    /* Set the opcode bits. */
    n_op = (short) (cmd->opcode) << 11;  /* Fill in opcode bits. */

    /* Combine the source, opcode, and destination bits to obtain the final unsigned short representation. */
    return ((n_op | n_src) | (n_dest | n_are));
}

unsigned short convert_reg_to_binary(command *cmd, int is_src) {
    static int already_done;
    int reg1, reg2;
    unsigned short n_reg_src, n_reg_dest;
    n_reg_src = n_reg_dest = 0;
    if (is_src) {
        /* if the register we are converting is a source argument */
        if ((reg1 = which_reg(cmd->cmd_src)) >= 0) {
            n_reg_src = reg1 << 7;
        }
        /* if the dest argument is also a register */
        if ((reg2 = which_reg(cmd->cmd_dst)) >= 0) {
            n_reg_dest = reg2 << 3;
        }
        already_done = 1; /* Indicating source & dest register has been dealt with or dest is not a register */
        /* return the combined value of both source & dest registers
         * if dest wasn't a register than '|' with zero doesn't change */
        return (n_reg_src | n_reg_dest);
    }
    /* Dealing with dest register, checking we didn't deal with it before we need to check if it's a register */
    else if (already_done == 0) {
        if ((reg2 = which_reg(cmd->cmd_dst)) >= 0) {
            n_reg_dest = reg2 << 3;
        }
        return n_reg_dest;
    }
    already_done = 0; /* resetting the static variable to zero for next line */
    /* we dealt with the dest register already so we return a value to indicate that and avoid doing it again */
    return 65535;
}

int add_information_line(converted_line **code_lines, command *cmd, int *IC, int is_src, int line) {
    unsigned short num;
    char *arg, *numeric_value[1000];

    /* Determine the argument based on whether the source or destination operand is being processed */
    arg = (is_src) ? cmd->cmd_src : cmd->cmd_dst;
    /* Check if the argument passed is a register, if yes set a numerical value for it,
     * The last condition checks if both source and dest are registers and therefore have been dealt together */
    if (which_reg(arg) >= 0 && (num = convert_reg_to_binary(cmd, is_src)) != 65535) {
        (*IC)++;
        /* Add the machine code line for the register representation */
        if (add_machine_code_cmd(code_lines, num, NULL, IC, line) == 0) {
            return 0;
        }
    }
    /* If arg is not a register it checks if it's a legal label and codes it */
    else if (validate_label(arg)) {
        (*IC)++;
        if (add_machine_code_line(code_lines, 1, arg, IC, line) == 0) {
            return 0;
        }
    }
    /* Check if the argument is a numerical value */
    else if (is_operand_number(arg)) {
        strcpy(numeric_value, arg + 1); 
        (*IC)++;
        /* MIGHT NEED TO USE 2ND COMPLEMENT */
        if (add_machine_code_line(code_lines, atoi(numeric_value) << ARE_BITS, NULL, IC, line) == 0) {
            return 0;
        }
    }
    /* added successfully or had nothing to add */
    return 1;

}