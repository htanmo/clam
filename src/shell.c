#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char *readline(void) {
  int bufsize = BUFSIZE;
  int pos = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "clam: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[pos] = '\0';
      return buffer;
    } else {
      buffer[pos] = c;
    }
    pos += 1;

    if (pos >= bufsize) {
      bufsize += BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "clam: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **split_lines(char *line) {
  int bufsize = TOK_BUFSIZE, pos = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!token) {
    fprintf(stderr, "clam: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[pos] = token;
    pos += 1;

    if (pos >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "clam: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[pos] = NULL;
  return tokens;
}

int launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("clam");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("clam");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// builtin shell commands
int cd(char **args);
int help(char **args);
int shell_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};

int (*builtin_func[])(char **) = {&cd, &help, &shell_exit};

int num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "clam: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("clam");
    }
  }
  return 1;
}

int help(char **args) {
  int i;
  printf("Omnath's clam");
  printf("Type program names and arguements, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < num_builtins(); i++) {
    printf(" %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int shell_exit(char **args) { return 0; }

int execute(char **args) {
  int i;
  
  if(args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}