#ifndef C_PROJECT_OPENU_GLOBALS_H
#define C_PROJECT_OPENU_GLOBALS_H

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 81

/* Default IC value */
#define IC_INIT_VALUE 100

/* arbitrary very big number for line length */
#define BIG_NUMBER_CONST 1000

#define OPCODES_CNT 16

#define REG_CNT 8

#define WORD_LEN 15

#define ARE_BITS 3

#define INSTR_CNT 4

#define MAX_NUM ((1 << (WORD_LEN-1)) - 1)

#define MIN_NUM (-(1 << (WORD_LEN-1)))

#endif
