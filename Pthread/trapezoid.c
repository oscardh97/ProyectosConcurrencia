#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int thread_count, n, local_n;
double a, b, h, total;
pthread_mutex_t mutex;

void *work(void *rank);
double Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen);
double f(double x);

double f(double x)
{
    return x * x * x;
}

double Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen)
{
    double estimate, x;

    estimate = (f(leftEndpt) + f(rightEndpt)) / 2;

    for (int i = 1; i <= trapCount - 1; ++i)
    {
        x = leftEndpt + i * baseLen;
        estimate += f(x);
    }

    estimate = estimate * baseLen;

    return estimate;
}

void *work(void *rank)
{
    double leftEndpt, rightEndpt, estimate;
    long my_rank = (long)rank;

    leftEndpt = a + my_rank * local_n * h;
    rightEndpt = leftEndpt + local_n * h;

    estimate = Trap(leftEndpt, rightEndpt, local_n, h);
    pthread_mutex_lock(&mutex);
    total += estimate;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t *master_thread;

    if (argc != 2)
    {
        fprintf(stderr, "Numero de threads: %s <number of threads>\n", argv[0]);
        exit(0);
    }

    thread_count = strtol(argv[1], NULL, 10);
    a = 0.0, b = 50000.0, n = 1024;
    h = (b - a) / n;
    local_n = n / thread_count;

    master_thread = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (long i = 0; i < thread_count; i++)
    {
        pthread_create(&master_thread[i], NULL, work, (void *)i);
    }

    for (long i = 0; i < thread_count; i++)
    {
        pthread_join(master_thread[i], NULL);
    }

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %19.15e\n", a, b, total);

    pthread_mutex_destroy(&mutex);
    free(master_thread);

    return 0;
}
