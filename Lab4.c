#include <stdio.h>

void keyboard_input(double &arr) {
    printf("Input x coordinates: ");
    for (int i = 0; i < 10; i++) {
        double x;
        scanf("%lf", &x);
        arr[0][i] = x;
    }

    printf("Input y coordinates: ");
    for (int i = 0; i < n; i++) {
        double y;
        scanf("%lf", &y);
        arr[1][i] = y;
    }
}

int main() {

    printf("Input the number of points:\n");
    scanf("%d", &n);
    double points[2][10];



}