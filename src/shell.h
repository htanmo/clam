#ifndef _SHELL_H_
#define _SHELL_H_
char *readline(void);
char **split_lines(char *line);
int launch(char **args);
int execute(char **args);
#endif
