#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int a = 0;

void signal_handler() {
    a++;
}

int main()
{
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        printf("\n not set");
    }

    for(int i = 0; i < 10000000; i++)
    {
        if (kill(getpid(), SIGUSR1) != 0) {
            perror("Sent ERROR");
        }
    }

    printf("%d\n", a);
}