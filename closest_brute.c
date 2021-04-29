#include <math.h>

#include "closest_brute.h"

double brute_force(struct Point P[], size_t n)
{
    double min_dist = INFINITY;
    double curr_dist;

    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            curr_dist = dist(P[i], P[j]);
            if (min_dist > curr_dist){
                min_dist = curr_dist;
            }
        }
    }

    return min_dist;
} 