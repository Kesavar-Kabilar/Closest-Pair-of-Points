#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "closest_parallel.h"

int curr_depth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{   
    static int num_forks = 0;

    if (n <= 3 || pdmax <= 0){
        return _closest_serial(P, n);
    }

    int p_mid = n / 2;

    int fd[2];

    pipe(fd);
    double dl;
    int r = fork();

    if(r > 0){
        close(fd[1]);
        int status;
        wait(&status);
        num_forks = WEXITSTATUS(status);
        if (WIFEXITED(status)){
            if(read(fd[0], &dl, sizeof(double)) == 0){
                perror("nothing to read");
            }

        }else{
            perror("child exited abnormally");
            exit(1);
        }
        close(fd[0]);
    }else if(r == 0){
        close(fd[0]);
        double dl_temp = _closest_parallel(P, p_mid, pdmax-1, pcount);
        if (write(fd[1], &dl_temp, sizeof(double)) == -1){
            perror("write to pipe");
            exit(1);
        }
        close(fd[1]);
        num_forks += 1;
        exit(num_forks);
    }

    pipe(fd);
    double dr;
    r = fork();

    if (r > 0){
        close(fd[1]);
        int status;
        wait(&status);
        num_forks = WEXITSTATUS(status);
        if (WIFEXITED(status)){
            if(read(fd[0], &dr, sizeof(double)) == 0){
                perror("nothing to read");
            }
        }else{
            perror("child exited abnormally");
            exit(1);
        }
        close(fd[0]);
    }else if (r == 0){
        close(fd[0]);
        double dr_temp = _closest_parallel(P+p_mid, n-p_mid, pdmax-1, pcount);
        if (write(fd[1], &dr_temp, sizeof(double)) == -1){
            perror("write to pipe error");
            exit(1);
        }
        close(fd[1]);
        num_forks += 1;
        exit(num_forks);
    }

    double d = (dl < dr)? dl : dr;

    *pcount = num_forks;

    return combine_lr(P, n, P[p_mid], d);
}

double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_parallel(P, n, pdmax, pcount);
}
