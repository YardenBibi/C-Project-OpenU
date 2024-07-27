#ifndef C_PROJECT_OPENU_ASSEMBLY_UTILS_H
#define C_PROJECT_OPENU_ASSEMBLY_UTILS_H

typedef struct opcode {
    char *opcode;    /* The opcode corresponding to the operation */
    int arg_num;     /* The number of arguments for the operation */
} opcode;

int name_as_inst(char *str);

int which_opcode(char *str);

int which_reg(char *str)