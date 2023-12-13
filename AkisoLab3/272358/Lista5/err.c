#include <stdio.h>

int main() {
    fprintf(stderr, "This is an intentional error message.\n");
    return 1;  // Return a non-zero exit code to indicate an error
}
