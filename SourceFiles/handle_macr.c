#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"

int macr_pre_process(char as_file[]){
    char *no_extra_space_file;

    no_extra_space_file = delete_extra_spaces(as_file);
    
}
