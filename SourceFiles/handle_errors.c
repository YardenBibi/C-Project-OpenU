#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"

void input_errors(char file_name[], int line, char error_msg[]){
    printf("Error in file %s, line %d:\n%s\n", file_name, line, error_msg);
}

void general_errors(char error_msg[]){
    printf("Error:\n%s\n", error_msg);
}