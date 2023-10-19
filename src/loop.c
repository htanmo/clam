#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

void loop(void) {
  char *line;
  char **args;
  int status;
  do {
    fprintf(stdout, "> ");
    line = readline();
    args = split_lines(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}