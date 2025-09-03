#include <stdio.h>

int main() {
    int A, B, C, D, E, F, res1, res2;

    printf("Input A, B, C, D, E, F values:\n");
    scanf("%d%d%d%d%d%d", &A, &B, &C, &D, &E, &F);


//    B * (C * E – D) / C + A * (F / D)


    asm(
        /* (C * E) - D */
        "movl %1, %%eax;"       /* C -> EAX */
        "movl %2, %%ebx;"       /* E -> EBX */
        "imull %%ebx;"          /* EAX * EBX -> EAX */
        "movl %3, %%ebx;"       /* D -> EBX*/
        "subl %%ebx, %%eax;"    /* EAX - EBX -> EAX */

        /* term1 = B * (C * E – D) / C */
        "movl %4, %%ebx;"       /* B -> EBX */
        "imull %%ebx;"          /* EAX * EBX -> EAX */
        "movl %1, %%ebx;"       /* C -> EBX */
        "idivl %%ebx;"          /* EAX / EBX -> EAX */
        "movl %%eax, %%ecx;"    /* EAX -> ECX */

        /* term2 = A * (F / D) */
        "movl %5, %%eax;"       /* F -> EAX */
        "movl %3, %%ebx;"       /* D -> EBX */
        "idivl %%ebx;"          /* EAX / EBX -> EAX*/
        "movl %6, %%ebx;"       /* A -> ABX */
        "imull %%ebx;"          /* EBX * EAX -> EAX */

        /* res1 = term1 + term2 */
        "addl %%ecx, %%eax;"    /* ECX + EAX -> EAX */
        "movl %%eax, %0;"       /* EAX -> res1 */
        : "=r" (res1)
        : "r" (C), "r" (E), "r" (D), "r" (B), "r" (F), "r" (A)
        : "%eax", "%ebx", "%ecx", "edx"
        );

    printf("'ASM' result: %d * (%d * %d - %d) / %d + %d * (%d / %d) = %d\n", B, C, E, D, C, A, F, D, res1);

    res2 = B * (C * E - D) / C + A * (F / D);
    printf("'C' result: %d * (%d * %d - %d) / %d + %d * (%d / %d) = %d", B, C, E, D, C, A, F, D, res2);


    return 0;
}