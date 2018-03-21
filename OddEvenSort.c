#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    char *p;
    int thread_count = strtol(argv[0], &p, 10);
    int a[100] = {66, 35, 46, 70, 90, 41, 59, 6, 27, 47, 14, 68, 50, 43, 46, 24, 76, 2, 80, 82, 62, 93, 42, 96, 53, 18, 39, 80, 17, 61, 2, 92, 99, 80, 28, 90, 9, 99, 68, 52, 48, 75, 6, 66, 25, 7, 54, 77, 84, 90, 68, 91, 13, 64, 94, 74, 9, 0, 6, 13, 49, 55, 68, 64, 20, 45, 54, 95, 20, 90, 65, 94, 89, 11, 31, 15, 56, 83, 20, 49, 43, 6, 94, 96, 21, 17, 48, 27, 26, 43, 16, 0, 19, 29, 79, 9, 16, 72, 64, 71};
    int n = 100;
    int i, tmp;
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            #pragma omp parallel for num_threads(thread_count) default(none) \
                shared(a, n) private(i, tmp)
            for (i = 1; i < n; i += 2) {
                if (a[i - 1] > a [i]) {
                    tmp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = tmp;
                }
            }
        } else {
            #pragma omp parallel for num_threads(thread_count) default(none) \
                shared(a, n) private(i, tmp)
            for (i = 1; i < n - 1; i += 2) {
                if (a[i] > a [i + 1]) {
                    tmp = a[i + 1];
                    a[i + 1] = a[i];
                    a[i] = tmp;
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        printf("a[%d] => %d\n", i, a[i]);
    }

    return 0;
}
