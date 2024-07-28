#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/handle_errors.h"

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

/* Define the instructions */
char *INSTUCTIONS[] = {".data", ".string", ".entry", ".extern"};

/* Define the registers */
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
