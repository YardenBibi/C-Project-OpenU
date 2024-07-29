#ifndef C_PROJECT_OPENU_HANDLE_MACR_H
#define C_PROJECT_OPENU_HANDLE_MACR_H

#include <stdio.h>
#include "globals.h"
#include "macr_node.h"

int macr_pre_process(char as_file[]);

int build_macros_list(macr_node **head_macr,char *am_file);

char* get_macr_code(char * file_name,FILE *file, int *line_number);

int is_valid_macr(int line, char *am_file, char **macr_name);

int ensure_macr_order(macr_node *head_macr, char *am_file);

int replace_macros(macr_node *head_macr, char *am_file);

int remove_macros(char *filename);

#endif
