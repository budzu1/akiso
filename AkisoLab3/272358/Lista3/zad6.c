//tput colors
#include <stdio.h>

int main()
{
    for(int i = 0; i < 256; i++)
    {
        printf("\x1B[38;5;%dmHello World!\n", i); //maybe [0m but idk if ness
    }

    return 0;
}