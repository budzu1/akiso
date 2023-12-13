#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = atoi(argv[1]);
    long orig_stdout;
    struct user_regs_struct regs;

    // Attach to the existing process
    if (ptrace(PTRACE_ATTACH, child_pid, NULL, NULL) == -1) {
        perror("ptrace");
        exit(EXIT_FAILURE);
    }

    // Wait for the process to stop
    waitpid(child_pid, NULL, 0);

    // Get the original stdout file descriptor
    orig_stdout = dup(STDOUT_FILENO);

    // Set the new file descriptor (stdout) to /tmp/foo
    if (freopen("/tmp/foo", "w", stdout) == NULL) {
        perror("freopen");
        exit(EXIT_FAILURE);
    }

    // Detach from the process
    if (ptrace(PTRACE_DETACH, child_pid, NULL, NULL) == -1) {
        perror("ptrace");
        exit(EXIT_FAILURE);
    }

    // Restore the original stdout file descriptor
    dup2(orig_stdout, STDOUT_FILENO);
    close(orig_stdout);

    printf("stdout redirected to /tmp/foo for process %d\n", child_pid);

    return 0;
}