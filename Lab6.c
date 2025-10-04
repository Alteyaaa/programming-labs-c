#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void keyboard_input(double (*arr)[100], int coord_num) {
    double x, y;

    printf("Input x and y coordinates:\n");
    for (int i = 0; i < coord_num; i++) {
        scanf("%lf %lf", &x, &y);
        *(*arr+i) = x;
        *(*(arr+1)+i) = y;
    }
    printf("\nGenerated points:\n");
    for (int i = 0; i < coord_num; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, *(*arr+i), *(*(arr+1)+i));
    }
}

void random_input(double (*arr)[100], int coord_num) {
    double min, max;
    printf("Input range boundaries:\n");
    scanf("%lf %lf", &min, &max);
    for (int i = 0; i < coord_num; i++) {
        *(*arr+i) = min + (max - min) * ((double) rand() / RAND_MAX);
        *(*(arr+1)+i) = min + (max - min) * ((double) rand() / RAND_MAX);
    }
    printf("\nGenerated points:\n");
    for (int i = 0; i < coord_num; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, *(*arr+i), *(*(arr+1)+i));
    }
}

int check_position_of_point(double arr_x, double arr_y, double R, double a) {
    int inside_circle = (pow(arr_x, 2) + pow(arr_y, 2)) <= pow(R, 2);
    int left_top = (arr_x <= 0) && (arr_y > 0) && (arr_y <= a);
    int right_bottom = (arr_x > 0) && (arr_y <= 0) && (arr_y >= -a);
    return (inside_circle && (left_top || right_bottom));
}

void print_results(double (*inside)[100], double (*outside)[100], int in_count, int out_count) {
    printf("\nPoints inside the area (%d points):\n", in_count);
    if (in_count > 0) {
        for (int i = 0; i < in_count; i++) {
            printf("Point %d: (%.2f, %.2f)\n", i + 1, *(*inside+i), *(*(inside+1)+i));
        }
    } else {
        printf("No points inside the area\n");
    }

    printf("\nPoints outside the area (%d points):\n", out_count);
    if (out_count > 0) {
        for (int i = 0; i < out_count; i++) {
            printf("Point %d: (%.2f, %.2f)\n", i + 1, *(*outside+i), *(*(outside+1)+i));
        }
    } else {
        printf("All points are inside the area!\n");
    }
}

int main() {
    double points[2][100];
    int n, coord_num;

    printf("Input number of coordinates:\n");
    scanf("%d", &coord_num);
    while (coord_num > 100 || coord_num < 1) {
        printf("The number of points is no more than 100 and no less than 1\n");
        scanf("%d", &coord_num);
    }

    printf("Select input variation:\n");
    printf("1. Keyboard input\n"
        "2. Random input from a given range\n");
    scanf("%d", &n);

    while (n != 1 && n != 2) {
        printf("Incorrect value entered\n");
        scanf("%d", &n);
    }
    if (n == 1) {
        keyboard_input(points, coord_num);
    } else {
        random_input(points, coord_num);
    }

    double R, a;
    printf("\nInput R of the circle and the boundary 'a' for Y:\n");
    scanf("%lf %lf", &R, &a);


    double inside[2][100], outside[2][100];
    int in_count = 0, out_count = 0;
    for (int i = 0; i < coord_num; i++) {
        double x = *(*points+i), y = *(*(points+1)+i);
        if (check_position_of_point(x, y, R, a)) {
            *(*inside+in_count) = x;
            *(*(inside+1)+in_count) = y;
            in_count++;
        } else {
            *(*outside+out_count) = x;
            *(*(outside+1)+out_count) = y;
            out_count++;
        }
    }

    print_results(inside, outside, in_count, out_count);

    return 0;
}
