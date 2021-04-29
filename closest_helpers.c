#include <math.h>

#include "closest_helpers.h"

int verbose = 0;

int compare_x(const void* a, const void* b) 
{
    const struct Point *x1 = a;
    const struct Point *x2 = b;

    return (x1->x) - (x2->x);
} 

int compare_y(const void* a, const void* b) 
{ 
    const struct Point *y1 = a;
    const struct Point *y2 = b;

    return (y1->y) - (y2->y);
} 

double dist(struct Point p1, struct Point p2) 
{   
    long int x = pow((p1.x-p2.x), 2);
    long int y = pow((p1.y-p2.y), 2);
    return sqrt(x + y);
} 
