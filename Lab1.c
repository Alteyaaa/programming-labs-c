#include <stdio.h>

int main() {
    int A, B, C, D, E, F, res1, res2;

    printf("Input A, B, C, D, E, F values:\n");
    scanf("%d%d%d%d%d%d", &A, &B, &C, &D, &E, &F);


    //    B * (C * E – D) / C + A * (F / D)


    asm(
        /* (C * E) - D */
        "movl %1, %%eax;"       /* C -> EAX */
        "imull %2;"             /* EAX * E -> EAX */
        "subl %3, %%eax;"       /* EAX - D -> EAX */

        /* term1 = B * (C * E – D) / C */
        "imull %4;"             /* EAX * B -> EAX */
        "cdq;"                  /* расширяем EAX до EDX:EAX для деления */
        "idivl %1;"             /* EDX:EAX / C -> EAX */
        "movl %%eax, %%ecx;"    /* EAX -> ECX */

        /* term2 = A * (F / D) */
        "movl %5, %%eax;"       /* F -> EAX */
        "cdq;"                  /* расширяем EAX до EDX:EAX для правильного деления */
        "idivl %3;"             /* EDX:EAX / D -> EAX*/
        "imull %6;"             /* A * EAX -> EAX */

        /* res1 = term1 + term2 */
        "addl %%ecx, %%eax;"    /* ECX + EAX -> EAX */
        "movl %%eax, %0;"       /* EAX -> res1 */
        : "=r" (res1)
        : "r" (C), "r" (E), "r" (D), "r" (B), "r" (F), "r" (A)
        : "%eax", "%ecx", "edx"
        );

    printf("'ASM' result: %d * (%d * %d - %d) / %d + %d * (%d / %d) = %d\n", B, C, E, D, C, A, F, D, res1);

    res2 = B * (C * E - D) / C + A * (F / D);
    printf("'C' result: %d * (%d * %d - %d) / %d + %d * (%d / %d) = %d", B, C, E, D, C, A, F, D, res2);

    // 1 7 5 7 2 4 проверка рез-тов
    // 10 2 3 4 2 1

    return 0;
}
