#include <stdio.h>

int main() {
    //     8.  Z= A * (B – C)
    // если Z=0, то вычислить C * B / (A + B)
    //                                 иначе вычислить A!
    int A, B, C, Z, result;
    printf("Input A, B, C values:\n");
    scanf("%d%d%d", &A, &B, &C);

    asm(
        // Z= A * (B – C)
        "movl %2, %%eax;"
        "subl %3, %%eax;"
        "imull %1;"
        "movl %%eax, %0"
        : "=r" (Z)
        : "r" (A), "r" (B), "r" (C)
        : "%eax"
    );

    printf("Z = %d * (%d - %d) = %d\n", A, B, C, Z);

    asm(
        "cmp $0, %4;"
        "jne not_equal;"

        //случай, если Z=0
        //C * B / (A + B)
        "movl %1, %%ebx;"
        "addl %2, %%ebx;"
        "cmp $0, %%ebx;" //проверяем, чтобы знаменатель не был равен 0
        "je end;"

        "movl %3, %%eax;"
        "imull %2;"
        "cdq;"
        "idivl %%ebx;"
        "movl %%eax, %0;" //считаем выражение C * B / (A + B)
        "jmp end;"

        //случай, если Z!=0
        "not_equal:"
        "movl %1, %%ecx;" //считаем факториал A
        "movl $1, %%eax;"

        "cmp $0, %%ecx;"
        "je end;"

        "loop_start:" //каждый раз домножаем счетчик
        "imull %%ecx;"
        "loop loop_start;"

        "movl %%eax, %0;"

        "end:"
        : "=r" (result)
        : "r" (A), "r" (B), "r" (C), "r" (Z)
        : "eax", "ecx", "ebx"
    );

    //Обработка некорректных значений
    if (Z == 0) {
        if (A + B == 0) {
            printf("Error:division by zero -> (A + B = 0)\n");
            printf("Cannot calculate: %d * %d / (%d + %d)\n", C, B, A, B);
        } else {
            printf("Result (Z=0): %d * %d / (%d + %d) = %d\n",
                   C, B, A, B, result);
        }
    } else {
        if (A < 0) {
            printf("Error: Cannot calculate factorial of negative number (%d)\n", A);
        } else if (A == 0) {
            printf("Result (Z!=0): %d! = 1\n", A);
        } else {
            printf("Result (Z!=0): %d! = %d\n", A, result);
        }
    }
}
