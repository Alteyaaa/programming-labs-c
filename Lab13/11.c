#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "11.h"

// Создание круга
Circle createCircle(const float x, const float y, const float r) {
    Circle circle;
    circle.x = x;
    circle.y = y;
    circle.radius = r;
    return circle;
}

// Проверка вхождения точки в круг
bool inCircle(const Circle *circle, const float x, const float y) {
    float x0 = circle->x;
    float y0 = circle->y;
    float r = circle->radius;
    return (pow(x - x0, 2) + pow(y - y0, 2)) <= pow(r, 2);
}

// Пересечение двух кругов
bool intersectionCirсles(const Circle *circle1, const Circle *circle2) {
    float dx = circle1->x - circle2->x;
    float dy = circle1->y - circle2->y;

    float distanceBetweenCenters = sqrtf(dx * dx + dy * dy);

    float sumRadiuses = circle1->radius + circle2->radius;

    return distanceBetweenCenters <= sumRadiuses;
}

// Пересечение круга с прямоугольником
bool intersectionCircleRectangle(const Circle *circle, const float x, const float y, const float width,
                                 const float height) {
    float closestX = fmaxf(x, fminf(circle->x, x + width));
    float closestY = fmaxf(y, fminf(circle->y, y + height));

    float distanceX = circle->x - closestX;
    float distanceY = circle->y - closestY;

    return (distanceX * distanceX + distanceY * distanceY) <= (circle->radius * circle->radius);
}

// Площадь пересечения двух кругов
float squareIntersectionCircles(const Circle *circle1, const Circle *circle2) {
    float dx = circle1->x - circle2->x;
    float dy = circle1->y - circle2->y;
    float d = sqrtf(dx * dx + dy * dy);

    float r1 = circle1->radius;
    float r2 = circle2->radius;

    if (d >= r1 + r2) return 0.0f; // нет пересечения
    if (d <= fabsf(r1 - r2)) {
        // один круг внутри другого
        float rmin = fminf(r1, r2);
        return M_PI * rmin * rmin;
    }

    float term1 = (r1 * r1 + d * d - r2 * r2) / (2 * r1 * d);
    float term2 = (r2 * r2 + d * d - r1 * r1) / (2 * r2 * d);

    // защита от выхода за пределы [-1,1] из-за ошибок округления
    term1 = fmaxf(-1.0f, fminf(1.0f, term1));
    term2 = fmaxf(-1.0f, fminf(1.0f, term2));

    float angle1 = 2 * acosf(term1);
    float angle2 = 2 * acosf(term2);

    float area1 = 0.5f * r1 * r1 * (angle1 - sinf(angle1));
    float area2 = 0.5f * r2 * r2 * (angle2 - sinf(angle2));

    return area1 + area2;
}

// Описанный круг вокруг треугольника
Circle circumscribedCircle(const float x1, const float y1,
                           const float x2, const float y2,
                           const float x3, const float y3, bool *error) {
    float A = x2 - x1;
    float B = y2 - y1;
    float C = x3 - x1;
    float D = y3 - y1;
    float E = A * (x1 + x2) + B * (y1 + y2);
    float F = C * (x1 + x3) + D * (y1 + y3);
    float G = 2 * (A * (y3 - y2) - B * (x3 - x2));
    if (fabs(G) < 1e-6) {
        // точки коллинеарны
        *error = true;
        return createCircle(0, 0, 0);
    }
    float cx = (D * E - B * F) / G;
    float cy = (A * F - C * E) / G;
    float r = hypot(cx - x1, cy - y1);
    *error = false;
    return createCircle(cx, cy, r);
}

// Минимальный охватывающий круг для массива кругов
Circle minimalEnclosingCircle(Circle *circles, int n) {
    if (n <= 0) return createCircle(0, 0, 0);
    float minX = circles[0].x - circles[0].radius;
    float maxX = circles[0].x + circles[0].radius;
    float minY = circles[0].y - circles[0].radius;
    float maxY = circles[0].y + circles[0].radius;
    for (int i = 1; i < n; i++) {
        if (circles[i].x - circles[i].radius < minX) minX = circles[i].x - circles[i].radius;
        if (circles[i].x + circles[i].radius > maxX) maxX = circles[i].x + circles[i].radius;
        if (circles[i].y - circles[i].radius < minY) minY = circles[i].y - circles[i].radius;
        if (circles[i].y + circles[i].radius > maxY) maxY = circles[i].y + circles[i].radius;
    }
    float cx = (minX + maxX) / 2;
    float cy = (minY + maxY) / 2;
    float maxR = 0;
    for (int i = 0; i < n; i++) {
        float dist = hypot(cx - circles[i].x, cy - circles[i].y) + circles[i].radius;
        if (dist > maxR) maxR = dist;
    }
    return createCircle(cx, cy, maxR);
}

