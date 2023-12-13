#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

void lsh_loop();
char* lsh_read_line(void);
char** lsh_split_line(char *line);
int lsh_launch(char **args, int run_in_background);
int lsh_execute(char **args);
int lsh_execute_piped(char **args, int num_pipes, int run_in_background);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_num_builtins();
void handle_sigint(int sig);
int lsh_launch_pipe(char **args, int fd, int which_ds);
void execute_pipeline(char ***commands, int num_commands);
void handle_sigquit(int sig);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
    lsh_loop();
    return EXIT_SUCCESS;
}

void lsh_loop()
{
    char *line;
    char **args;
    int status;

    do {
        printf("my (not copied from link) lsh > ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char* lsh_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;  // have getline allocate a buffer for us
    if (getline(&line, &bufsize, stdin) == -1)
        exit(EXIT_SUCCESS);
    return line;
}

char** lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int lsh_launch(char **args, int run_in_background)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process

        signal(SIGINT, SIG_DFL);

        // Check for input/output redirection
        int i;
        for (i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                freopen(args[i + 1], "r", stdin);
                args[i] = NULL;
            } else if (strcmp(args[i], ">") == 0) {
                freopen(args[i + 1], "w", stdout);
                args[i] = NULL;
            } else if (strcmp(args[i], "2>") == 0) {
                freopen(args[i + 1], "w", stderr);
                args[i] = NULL;
            }
        }

        if (execvp(args[0], args) == -1) {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        if (!run_in_background) {
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else{
            printf("ran bg\n");
        }
    }

    return 1;
}
int lsh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args)
{
    int i;
    printf("Tomasz Budzynski or not idk some things are changed.\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < lsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char **args)
{
    return 0;
}

int lsh_execute(char **args)
{
    int i;
    int run_in_background = 0;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    // Check if the last argument is "&"
    int num_args = 0;
    while (args[num_args] != NULL) {
        num_args++;
    }

    if (num_args > 1 && strcmp(args[num_args - 1], "&") == 0) {
        run_in_background = 1;
        args[num_args - 1] = NULL;  // Remove "&" from the arguments
        num_args --;
    }

    // Check for pipes
    int num_pipes = 0;
    for (i = 0; i < num_args; i++) {
        if (strcmp(args[i], "|") == 0) {
            num_pipes++;
        }
    }

    if (num_pipes > 0) {
        char ***commands = malloc((num_pipes + 1) * sizeof(char **));
        if (commands == NULL) {
            perror("lsh: allocation error");
            exit(EXIT_FAILURE);
        }

        int cmd_start = 0;
        int cmd_count = 0;

        for (i = 0; i < num_args; i++) {
            if (strcmp(args[i], "|") == 0) {
                args[i] = NULL;
                commands[cmd_count++] = args + cmd_start;
                cmd_start = i + 1;
            }
        }
        commands[cmd_count] = args + cmd_start;

        execute_pipeline(commands, num_pipes + 1);
        free(commands);
        return 1;
    } else {
        printf("not running pipe\n");
        // Regular command without pipes
        for (i = 0; i < lsh_num_builtins(); i++) {
            if (strcmp(args[0], builtin_str[i]) == 0) {
                return (*builtin_func[i])(args);
            }
        }

        return lsh_launch(args, run_in_background);
    }
}


void execute_pipeline(char ***commands, int num_commands) {
    int i;

    int **pipes = malloc((num_commands - 1) * sizeof(int *));
    for (i = 0; i < num_commands - 1; i++) {
        pipes[i] = malloc(2 * sizeof(int));
        if (pipe(pipes[i]) == -1) {
            perror("lsh: pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t *pids = malloc(num_commands * sizeof(pid_t));
    for (i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("lsh: fork failed");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            // Child process
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }

            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(commands[i][0], commands[i]);
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (i = 0; i < num_commands; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }

    free(pids);
    free(pipes);
}

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

void handle_sigint(int sig) {
    printf("\n");
}

void handle_sigquit(int sig)
{
    exit(0);
}