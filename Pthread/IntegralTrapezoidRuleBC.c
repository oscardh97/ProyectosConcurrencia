#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int threadCount, n, localN;
double a, b, h, total;

void *getLocalInt(void *rank);
double Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen);
double f(double x);
int flag;

double f(double x) {
    return x * x * x;
}

double Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen) {
    double estimate, x;

    estimate = (f(leftEndpt) + f(rightEndpt)) / 2;

    for (int i = 1; i <= trapCount - 1; ++i) {
        x = leftEndpt + i * baseLen;
        estimate += f(x);
    }

    estimate = estimate * baseLen;

    return estimate;
}

void *getLocalInt(void *rank) {
    double leftEndpt, rightEndpt, estimate;
    long my_rank = (long)rank;

    leftEndpt = a + my_rank * localN * h;
    rightEndpt = leftEndpt + localN * h;

    estimate = Trap(leftEndpt, rightEndpt, localN, h);
    while (flag == ((int)rank));
    total += estimate;

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t *master_thread;
    flag = 0;
    if (argc != 2) {
        fprintf(stderr, "Total threads: %s <number of threads>\n", argv[0]);
        exit(0);
    }

    threadCount = strtol(argv[1], NULL, 10);
    a = 0.0, b = 50000.0, n = 1024;
    h = (b - a) / n;
    localN = n / threadCount;

    master_thread = malloc(threadCount * sizeof(pthread_t));

    for (long i = 0; i < threadCount; i++) {
        pthread_create(&master_thread[i], NULL, getLocalInt, (void *)i);
    }

    for (long i = 0; i < threadCount; i++) {
        pthread_join(master_thread[i], NULL);
    }

    printf("With %d trapezoids\n", n);
    printf("of the integral from %f to %f = %19.15e\n", a, b, total);

    free(master_thread);

    return 0;
}
