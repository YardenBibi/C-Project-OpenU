#include <stdio.h>
#include <stdlib.h>
#include "../HeaderFiles/common.h"



int main (int argc, cher *argv[]){
  int i; 
  char *dot_as_file, *current_file;
  for (i=1; i<argc;i++){
    current_file = argv[argc];
    dot_as_file = generate_file_name(current_file, ".as");


    printf("handle .as files (split the macros)");
    printf("continue if fails");
    printf("Perform first+second pass");
  
  }
}
