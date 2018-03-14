#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

//Pthread trapezoidal rule con semaforos y pthreads

int thread_count, n, local_n;
double a, b, h, total;
sem_t semaphore;

void *work(void *rank);
double Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen);
double f(double x);

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

void *work(void *rank) {
    double leftEndpt, rightEndpt, estimate;
    long my_rank = (long)rank;

    leftEndpt = a + my_rank * local_n * h;
    rightEndpt = leftEndpt + local_n * h;

    estimate = Trap(leftEndpt, rightEndpt, local_n, h);

    sem_wait(&semaphore);
    total += estimate;

    sem_post(&semaphore);

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t *master_thread;

    thread_count = strtol(argv[1], NULL, 10);
    a = 0.0, b = 50000.0, n = 1024;
    h = (b - a) / n;
    local_n = n / thread_count;

    master_thread = malloc(thread_count * sizeof(pthread_t));
    sem_init(&semaphore, 0, 1);

    for (long i = 0; i < thread_count; i++) {
        pthread_create(&master_thread[i], NULL, work, (void *)i);
    }

    for (long i = 0; i < thread_count; i++) {
        pthread_join(master_thread[i], NULL);
    }

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %19.15e\n", a, b, total);

    sem_destroy(&semaphore);
    free(master_thread);

    return 0;
}
