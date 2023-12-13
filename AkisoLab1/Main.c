#include <stdio.h>

#include "MyGcd.h"

int main ()
{
    // numbers to GCD
    int a;
    int b;

    printf ("Calculate GCD\nGive 2 possitive Integers:\n");
    scanf ("%d", &a);
    scanf ("%d", &b);

    int result = myGcd (a, b);

    printf ("Result: %d\n", result);

    return 0;
}