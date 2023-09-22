#include <stdio.h>

int main() {
    int sum = 0;

    printf("\n");
    for(int i = 1; i < 11; i++) {
        sum += i;
        printf("The current sum is: %d\n", sum);
    }
    printf("\n");
}