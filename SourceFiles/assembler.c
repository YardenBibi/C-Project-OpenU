#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_macr.h"



int main (int argc, char *argv[]){
  int i; 
  char *dot_as_file, *current_file;
  for (i=1; i<argc;i++){
    current_file = argv[i];
    dot_as_file = generate_file_name(current_file, ".as");
    if (!access(dot_as_file,F_OK)==0) {
      printf("The input %s does not exist, IGNORING\n", dot_as_file);
      continue;
    }
    if (!macr_pre_process(dot_as_file)){
      continue;
    }
    printf("Perform first+second pass\n");
  
  }
  return 0;
}
