#include <stdio.h>
#include <math.h>

float f(float x) {
    return 2 * x * x - 5 * x + 1;
}

float recursively(float a, float b, float eps) {
    float x = (b + a) / 2;
    if (fabs(b - a) < eps || fabs(f(x)) < eps) {
        return x;
    }
    if (f(a) * f(x) < 0) {
        return recursively(a, x, eps);
    } else {
        return recursively(x, b, eps);
    }
}

float iteratively(double a, double b, double eps) {
    float x = (b + a) / 2;;
    while (fabs(b - a) > eps || fabs(f(x)) > eps) {
        x = (b + a) / 2;
        if (f(a) * f(x) < 0) {
            b = x;
        } else {
            a = x;
        }
    }
    x = (b + a) / 2;
    return x;
}

int main() {
    float a, b, eps;
    printf("Input [a, b] value >> ");
    scanf("%f %f", &a, &b);
    while (f(a) * f(b) > 0) {
        printf("More than one root or no roots on the segment!\n");
        printf("Input [a, b] value >> ");
        scanf("%f %f", &a, &b);
    }

    printf("Input epsilon value >> ");
    scanf("%f", &eps);

    printf("Iteratively answer: %f\n", iteratively(a, b, eps));
    printf("Recursively answer: %f", recursively(a, b, eps));
}
