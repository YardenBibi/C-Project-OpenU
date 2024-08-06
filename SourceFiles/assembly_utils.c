#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/common.h"

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
char *REGS[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"}; 

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

    for (i = 0; i < REG_CNT; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            return i; 
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