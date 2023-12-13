#include <assert.h>
#include <stdio.h>

#include "MyGcd.h"

int main ()
{
    // number of tests
    const int  nt = 5;

    printf ("Test 1/%d\n", nt);
    assert (myGcd (5, 13) == 1);

    printf ("Test 2/%d\n", nt);
    assert (myGcd (10, 6) == 2);

    printf ("Test 3/%d\n", nt);
    assert (myGcd (15, 5) == 5);

    printf ("Test 4/%d\n", nt);
    assert (myGcd (75, 100) == 25);

    printf ("Test 5/%d\n", nt);
    assert (myGcd (18, 24) == 6);

    printf ("Everything have gone well\n");
}