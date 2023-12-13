#ifndef MYGCD
#define MYGCD

int myGcd (int a, int b)
{
    if (a == 0)
    {
        return b;
    }

    return myGcd (b % a, a);
}

#endif