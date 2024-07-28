#ifndef C_PROJECT_OPENU_HANDLE_MACR_H
#define C_PROJECT_OPENU_HANDLE_MACR_H

#include <stdio.h>
#include "globals.h"
#include "macr_node.h"

/*טפל בכל החלק של המאקרו כולל מקרי קצה כפי בנדרש בקובץ עבודה*/
int macr_pre_process(char as_file[]);

int build_macros_list(macr_node **head_macr,char *no_extra_space_file);

char* get_macr_code(char * file_name,FILE *file, int *line_number);

int is_valid_macr(int line, char *no_extra_space_file, char **macr_name);

int ensure_macr_order(macr_node *head_macr, char *no_extra_space_file);

#endif
