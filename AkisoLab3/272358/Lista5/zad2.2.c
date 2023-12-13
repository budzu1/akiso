#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    if (kill(1, SIGKILL) == 0) {
        printf("SIGKILL sent\n");
    } else {
        perror("Error");
        return 1;
    }

    return 0;
}
