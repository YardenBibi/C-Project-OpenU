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
    if ((!isalpha(*label)) & !(isdigit(*label))){return 0;}
    while (*(++label) != '\0' && *(label) != ' ') {
        if ((isalpha(*label)) || (isdigit(*label))){
            continue;
        }
        return 0;
    }
    return 1;
}

int validate_label_declaration(char *label){
    if (label == NULL || (strlen(label) > MAX_LABEL_LENGTH) || \
        (which_opcode(label) >= 0) || (name_as_inst(label)) || \
        (which_reg(label)>=0) || !(isalpha(*label))){
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
    char buffer_copy[MAX_LINE_LENGTH],*label,*ptr;
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
    char buffer_copy[MAX_LINE_LENGTH],*label,*colon_pos;
    strcpy(buffer_copy,buffer);

    inst = manual_malloc(sizeof(instruction));
    if (inst == NULL) {
        return NULL;
    }

    colon_pos = strchr(buffer_copy, ':');  
    if (colon_pos != NULL) {
        if (*(colon_pos + 1) != ' ' && *(colon_pos + 1) != '\0') {
            memmove(colon_pos + 2, colon_pos + 1, strlen(colon_pos + 1) + 1);  /* Shift the rest of the string right by 1 */
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
        if(!build_string(label,inst, error_msg)){
            return NULL;
        }
    }

    return inst;
}

int build_array(char *buffer, instruction *inst, char *error_msg){
    char *array_str, *num;
    int arr_len=1, number, digitFlag = 0, commaFlag = 0, signFlag = 0, i, spaceFlag=0;

    
    array_str = strtok(NULL, "\n");
    if (!isdigit(array_str[0]) && array_str[0] != '-'){return 0;}
    for (i = 0; array_str[i]; i++) {
        if(array_str[i] == ' '){
                spaceFlag=1;
                continue;
        }
        if (array_str[i] == ',') {
            if (!digitFlag || commaFlag) {
                strcpy(error_msg, "Illegal .data instruction");
                return 0; 
            }
            spaceFlag=0;
            arr_len++;
            commaFlag = 1;
            signFlag=0;
            digitFlag = 0;
        }
        else if (isdigit(array_str[i]) || array_str[i] == '-' || array_str[i] == '+') {
            if (spaceFlag && !commaFlag){
                strcpy(error_msg, "Illegal .data instruction");
                return 0; 
            }         
            commaFlag = 0;
            spaceFlag=0;
            if (array_str[i] == '-' || array_str[i] == '+') {
                if (signFlag || digitFlag ){return 0;}
                signFlag = 1;
                digitFlag = 0;
                continue;
            }
            if (isdigit(array_str[i])) {
                digitFlag = 1;
                signFlag=0;
            }
        } else {
            strcpy(error_msg, "Illegal .data instruction");
            return 0; 
        }
    }
    if (commaFlag) {return 0;}
    inst->len = arr_len;
    inst->nums = (short *)malloc(inst->len * sizeof(short));
    i=0;
    num = strtok(array_str, ",");
    number = (short) (atoi(num));
        if (number > MAX_NUM || number < MIN_NUM) {
            strcpy(error_msg, "Illegal .data instruction - a number is too big");
            return 0;
        } 
        inst->nums[i++]= (short) (atoi(num));
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
    printf("BUILDING STRING!!!\n");
    string = strtok(NULL, "\n");
    printf("STRING:%s.\n",string);
    if (string[0] != '"') {
        strcpy(error_msg, "Illegal .string declaration - no opening quote");
        return 0;
    }
    for (i = 1; string[i] != '\0'; i++) { 
        if (string[i] == '"') {
            second_pos = i;
            break;
        }
    }
    if (string[second_pos+1] != '\0' && string[second_pos+1] != '\n') {
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
    for (i = 1; i < inst->len; i++) { 
        inst->nums[i - 1] = (short)string[i];
    }
    printf("FINISHED BUILDING STRING!!!\n");
    inst->nums[str_len] = 0;
    return 1;
}

int coma_more_than_one(char *str) {
    int cnt= 0 ;
    char *copy;
    copy = str;

    while (*copy) {
        if (*copy == ','){
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
    printf("STR:%s\n",str);
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
    return ((which_reg(arg) >= 0 && arg[0]!='*') || validate_label(arg) || is_operand_number(arg));
}

void add_machine_code_inst(converted_line **inst_lines, instruction *inst, int *DC, int line){
    int i;
    i = 0;
    for (i = 0; i < inst->len; i++) {
        if (inc_mem(inst_lines, *DC) == 0) {
            return ;
        }
        (*inst_lines + *DC)->short_num = *(inst->nums + i);
        (*inst_lines + *DC)->label = NULL;
        (*inst_lines + *DC)->line_num = line;
        (*DC)++;
    }
}

int add_machine_code_cmd(converted_line **code_lines, unsigned short num, char *str, int *IC, int line) {
    if (inc_mem(code_lines, *IC) == 0) {
        return 0;  
    }

    (*code_lines + *IC)->short_num = num;
    (*code_lines + *IC)->line_num = line;
    if (str == NULL) {
        (*code_lines + *IC)->label = NULL;  
    } else {
        (*code_lines + *IC)->label = manual_malloc((strlen(str) + 1) * sizeof(char));
        if ((*code_lines + *IC)->label == NULL) {
            return 0;  
        }
        strcpy((*code_lines + *IC)->label, str);  
    }
    printf("ADDED MACHINE CODE %d LINE %d\n",num,line);
    return 1; 
}

int inc_mem(converted_line **lines, int counter) {
    converted_line *ptr;
    ptr = *lines;
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
    command *cmd = manual_malloc(sizeof(command));
    if (cmd == NULL) {
        return NULL;
    }
    cmd->already_done = 0;
    buffer_copy = (char *)malloc(strlen(buffer) + 1);
    strcpy(buffer_copy, buffer);
    cmd_part = strtok(buffer_copy, " \n");
    printf("cmdpart: %s\n",cmd_part);
    colon_pos = strchr(buffer_copy, ':');
    if (colon_pos != NULL) {
        if (*(colon_pos + 1) != ' ' && *(colon_pos + 1) != '\0') {
            memmove(colon_pos + 2, colon_pos + 1, strlen(colon_pos + 1) + 1);  
            *(colon_pos + 1) = ' ';  
        }
        label = buffer_copy;
        if (!validate_label_declaration(label)) {
            strcpy(error_msg, "Illegal label declaration");
            free(cmd);
            return NULL;
        }
        cmd->cmd_label = label;
        cmd_part = strtok(NULL, " \n");
    }
    else{cmd->cmd_label = NULL;}
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
        sprintf(error_msg, "Arguments are misssing for the opcode %d",cmd->opcode );
        free(cmd);
        return NULL;
    }
    if (OPCODES[cmd->opcode].arg_num == 1) {
        if (strtok(NULL, " \n")!=NULL) {
            strcpy(error_msg, "Extra text after arguments");
            free(cmd);
            return NULL;
        }
    }
    if (OPCODES[cmd->opcode].arg_num == 2) {
        if (strstr(cmd_part, ",") == NULL) {
            sprintf(error_msg, "Missing comma for opcode %d with two arguents",cmd->opcode);
            free(cmd);
            return NULL;
        } 
        if (coma_more_than_one(cmd_part)) {
            sprintf(error_msg, "Extra comma in opcode %d with two arguents",cmd->opcode);
            free(cmd);
            return NULL;
        }

        arg1 = strtok(cmd_part, ",");
        printf("ARG1: %s\n",arg1);
        if (strchr(arg1, ' ')) {
            
            /*HANDLE IF THERES A TRAILING SPACE*/
        }
        if (!is_legal_operand(arg1)) {
            sprintf(error_msg, "The first operand '%s' isn't legal",arg1);
            free(cmd);
            return NULL;
        }
        printf("legal arg1\n");
        strtok(buffer, ",");
        arg2 = strtok(NULL," \n");
        printf("ARG2: %s.\n",arg2);
        if (strtok(NULL, "\n")) {
            strcpy(error_msg, "Extra text after arguments");
            free(cmd);
            return NULL;
        }
        if (!is_legal_operand(arg2)) {
            sprintf(error_msg, "The operand '%s' isn't legal",arg2);
            free(cmd);
            return NULL;
        }
        printf("legal arg2\n");
    }
    printf("Before switch-opcode:%d\n",cmd->opcode);
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
                sprintf(error_msg, "In the given opcode %d - The operand %s \
                isn't label or register", cmd->opcode, arg2);
                free(cmd);
                return NULL;
            }
            break;
        }
        case 4:{
            if (!validate_label(arg1)){
                sprintf(error_msg, "In the given opcode %d - The operand %s isn't a label", cmd->opcode, arg2);
                free(cmd);
                return NULL;
            }
            
            if (!((which_reg(arg2) >= 0) || validate_label(arg2))) {
                sprintf(error_msg, "In the given opcode %d - The operand %s \
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
                sprintf(error_msg, "In the given opcode %d - The operand %s \
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
            if ((which_reg(cmd_part) >= 0 && cmd_part[0] != '*') || validate_label(cmd_part)) {
                cmd->cmd_src = NULL;
                cmd->cmd_dst = cmd_part;
            } else {
                sprintf(error_msg, "In the given opcode %d - The operand %s \
                isn't valid", cmd->opcode, cmd_part);           
                free(cmd);
                return NULL;
            }
            break;
        }

        default :{
            sprintf(error_msg, "The given opcode %d isn't valid", cmd->opcode);
            free(cmd);
            return NULL;
        }
    }
    printf("created cmd\n");
    return cmd;
}



unsigned short convert_reg_to_binary(command *cmd, int is_src) {
    int reg1, reg2;
    unsigned short n_reg_src, n_reg_dest;
    n_reg_src = n_reg_dest = 0;
    printf("HERE WITH CMD:%s. ALREADY:%d\n",cmd->cmd_dst,cmd->already_done);
    if (is_src) {
        if ((reg1 = which_reg(cmd->cmd_src)) >= 0) {
            n_reg_src = reg1 << 6;
        }
        if ((reg2 = which_reg(cmd->cmd_dst)) >= 0) {
            n_reg_dest = reg2 << ARE_BITS;
        }
        cmd->already_done = 1; 
        return (n_reg_src | n_reg_dest);
    }
    else if (cmd->already_done == 0) {
        if ((reg2 = which_reg(cmd->cmd_dst)) >= 0) {
            n_reg_dest = reg2 << 3;
        }
        return n_reg_dest;
    }
    return 65535;
}

int add_information_line(converted_line **code_lines, command *cmd, int *IC, int is_src, int line, unsigned short n_are) {
    unsigned short num;
    char *arg, numeric_value[1000];
    arg = (is_src) ? cmd->cmd_src : cmd->cmd_dst;
    printf("ARG:%s.\n",arg);
    if (which_reg(arg) >= 0 && (num = convert_reg_to_binary(cmd, is_src)) != 65535) {
        printf("REG WITH ARG:%s.\n",arg);
        (*IC)++;
        num = num | n_are;
        if (add_machine_code_cmd(code_lines, num, NULL, IC, line) == 0) {
            return 0;
        }
    }
    else if (which_reg(arg)==-1 && validate_label(arg)) {
        printf("LABEL WITH ARG:%s.\n",arg);
        (*IC)++;
        if (add_machine_code_cmd(code_lines, 2, arg, IC, line) == 0) {
            return 0;
        }
    }
    else if (is_operand_number(arg)) {
        printf("NUMBER WITH ARG:%s.\n",arg);
        strcpy(numeric_value, arg + 1); 
        (*IC)++;
        if (add_machine_code_cmd(code_lines, (atoi(numeric_value) << ARE_BITS) | n_are, NULL, IC, line) == 0) {
            return 0;
        }
    }
    return 1;

}

int merge_all_lines(converted_line **code_lines, converted_line *data, int IC, int DC) {
    int i;
    if (inc_mem(code_lines, IC + DC) == 0) {
        return 0;
    }
    for (i = 0; i < DC; i++) {
        (*code_lines + IC + i + 1)->label = (data + i)->label;
        (*code_lines + IC + i + 1)->line_num = (data + i)->line_num;
        (*code_lines + IC + i + 1)->short_num = (data + i)->short_num;
    }
    return 1; 
}

void create_ob_file(converted_line *code_lines,int IC, int DC,char *filename){
    char* ob_file, error_buffer[INITIAL_BUFFER_SIZE];
    int i;
    FILE *file;
    ob_file = generate_file_name(filename, ".ob");
    
    file = fopen(ob_file,"w");
    if (file==NULL){
        sprintf(error_buffer,"Couldn't open file %s", ob_file);
        general_errors(error_buffer);
        return ;
    }
    fprintf(file, "  %d %d\n", IC + 1, DC);
    for (i = 0; i < IC + DC; i++) {
        fprintf(file, "%04d %05o\n", i + IC_INIT, code_lines[i].short_num & 0x7FFF);
    }
    fprintf(file, "%04d %05o", i + IC_INIT, code_lines[i].short_num & 0x7FFF);
    fclose(file);
}

void free_machine_code(converted_line *code_lines, int IC_DC){
    int i;
    for (i = 0; i <= IC_DC; i++) {
        if (code_lines[i].label != NULL) {
            free(code_lines[i].label);
        }
    }
    free(code_lines);
}