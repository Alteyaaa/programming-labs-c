#ifndef LAB11_H
#define LAB11_H

typedef struct {
    float x;
    float y;
    float radius;
} Circle;

Circle createCircle(const float x, const float y, const float r);

bool inCircle(const Circle *circle, const float x, const float y);

bool intersectionCirсles(const Circle *circle1, const Circle *circle2);

bool intersectionCircleRectangle(const Circle *circle, const float x, const float y, const float width,
                                 const float height);

float squareIntersectionCircles(const Circle *circle1, const Circle *circle2);

Circle circumscribedCircle(const float x1, const float y1,
                           const float x2, const float y2,
                           const float x3, const float y3, bool *error);

Circle minimalEnclosingCircle(Circle *circles, int n);


#endif
