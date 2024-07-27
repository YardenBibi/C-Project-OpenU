#ifndef C_PROJECT_OPENU_HANDLE_MACR_H
#define C_PROJECT_OPENU_HANDLE_MACR_H

#include <stdio.h>
#include "globals.h"

/*טפל בכל החלק של המאקרו כולל מקרי קצה כפי בנדרש בקובץ עבודה*/
int macr_pre_process(char as_file[]);


int build_macr_list(node **head_macr,char *no_extra_space_file);

char *get_macr_code(FILE file, int *line);

#endif


