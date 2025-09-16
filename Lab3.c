#include <float.h>
#include <stdio.h>
#include <math.h>

double function1(int nm1, double x, double y) {
    double sum = 0;
    for (int n = 0; n <= nm1; n++) {
        sum += (x + 2 * pow(y, n + 1)) / pow((2 * y + 1), n);
    }
    return sum;
}

double function2(int nm2, int a, double x, double y) {
    double sum = 0;
    for (int n = 0; n <= nm2; n++) {
        // log() сам вернет NaN для недопустимых аргументов
        sum += log(y * x) * (sin(x - a) + n);
    }
    return sum;
}

int main() {
    double x0, hx, xn, y0, hy, yn;
    int a, nm1, nm2;

    printf("Enter x0, hx, xn:\n");
    scanf("%lf %lf %lf", &x0, &hx, &xn);

    if (x0 > xn || hx <= 0) {
        printf("Invalid data for tabulation by x\n");
        return 1;
    }

    printf("Enter y0, hy, yn:\n");
    scanf("%lf %lf %lf", &y0, &hy, &yn);

    if (y0 > yn || hy <= 0) {
        printf("Invalid data for tabulation by y\n");
        return 1;
    }

    printf("Enter a, nm1, nm2:\n");
    scanf("%d %d %d", &a, &nm1, &nm2);

    if (nm1 < 2 || nm1 > 6 || nm2 < 2 || nm2 > 6) {
        printf("nm1 and nm2 must be in [2,6]\n");
        return 1;
    }

    printf("+-----------+-----------+--------------+\n");
    printf("|     x     |     y     |    f(x,y)    |\n");
    printf("+-----------+-----------+--------------+\n");


    double f, fmax = -DBL_MAX, fmin = DBL_MAX;
    double xmax, xmin, ymax, ymin;


    for (double y = y0; y <= yn; y += hy) {
        for (double x = x0; x <= xn; x += hx) {
            if (y < a) {
                f = function1(nm1, x, y);
            } else {
                f = function2(nm2, a, x, y);
            }
            printf("| %9.3f | %9.3f | %12.3f |\n", x, y, f);

            if (f > fmax) {
                fmax = f;
                xmax = x;
                ymax = y;
            }
            if (f < fmin) {
                fmin = f;
                xmin = x;
                ymin = y;
            }
        }
    }

    printf("+-----------+-----------+--------------+\n");

    printf("The maximum value of f(%.3f) is achieved at x=%.3f and y=%.3f\n", fmax, xmax, ymax);
    printf("The minimum value of f(%.3f) is achieved at x=%.3f and y=%.3f", fmin, xmin, ymin);

    return 0;
}