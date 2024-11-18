// print_functions.c
#include <stdio.h>

void PrintInt(int value) {
    printf("%d\n", value);
}

void PrintReal(double value) {
    printf("%f\n", value);
}

void PrintBoolean(int value) {
    if (value) {
        printf("true\n");
    } else {
        printf("false\n");
    }
}
