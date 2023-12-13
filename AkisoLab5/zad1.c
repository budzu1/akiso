#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void makeRooted(void)
{
    if (setuid(0) != 0) {
        perror("Error changing user ID");
        exit(1);
    }
}

int main() {

    makeRooted();

    system("/bin/bash");

    return 0;
}