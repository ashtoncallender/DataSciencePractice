#include <stdio.h>

int main() {
    int fact = 1;

    printf("\n");
    for (int i = 1; i < 11; i++) {
        fact *= i;
        printf("The current factorial is: %d\n", fact);
    }

    printf("\n");
}