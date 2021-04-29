#include <stdlib.h>
#include <stdio.h>
#include "closest_serial.h"

double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d)
{
    struct Point *close = malloc(n * sizeof(struct Point));
    int close_index = 0;
    for(int i = 0; i < n; i++){
        if(abs(P[i].x - mid_point.x) < d){
            close[close_index] = P[i];
            close_index +=1 ;
        }
    }

    qsort(close, close_index, sizeof(struct Point), compare_y);

    double smallest_lr = d;

    for (int i = 0; i < close_index; i++){
        for (int j = i+1; j < close_index && ((close[j].y - close[i].y) < d) ; j++){
            if (dist(close[i],close[j]) < smallest_lr){
                smallest_lr = dist(close[i], close[j]);
            }
        }
    }
    free(close);
    return smallest_lr;
}
double _closest_serial(struct Point P[], size_t n)
{
    if (n <= 3){
        return brute_force(P, n);
    }

    size_t p_mid = n / 2;

    double dl = _closest_serial(P, p_mid);
    double dr = _closest_serial(P+p_mid, n-p_mid);
    double d = (dl < dr)? dl : dr;
    return combine_lr(P, n, P[p_mid], d);
}

double closest_serial(struct Point P[], size_t n)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_serial(P, n);
}
