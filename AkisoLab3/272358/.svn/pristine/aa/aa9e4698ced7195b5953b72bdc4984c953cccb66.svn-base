#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signal_number) {
    printf("Got signal: %d\n", signal_number);
}

void set_signal_handler(void)
{
    for (int i = 1; i < _NSIG; i++) {
        if (signal(i, signal_handler) == SIG_ERR) {
            printf("%d\n not set", i);
        }
        printf("%d set\n", i);
    }
}

int main() {
    
    set_signal_handler();

    printf("Program oczekuje na sygnały. PID: %d\n", getpid());

    int i;

    for (i = 1; i < 9; i++) {
        // Wysyłanie sygnału do samego siebie
        if (kill(getpid(), i) != 0) {
            perror("Błąd podczas wysyłania sygnału");
            // exit(EXIT_FAILURE);
        }
    }

    for (i = 10; i < 19; i++) {
        // Wysyłanie sygnału do samego siebie
        if (kill(getpid(), i) != 0) {
            perror("Błąd podczas wysyłania sygnału");
            // exit(EXIT_FAILURE);
        }
    }

    for (i = 20; i < _NSIG; i++) {
        // Wysyłanie sygnału do samego siebie
        if (kill(getpid(), i) != 0) {
            perror("Błąd podczas wysyłania sygnału");
            // exit(EXIT_FAILURE);
        }
    }

    return 0;
}
