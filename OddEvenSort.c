#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    char *p;
    int thread_count = strtol(argv[0], &p, 10);
    int a[100] = {7,33,84,40,37,34,46,52,48,93,27,83,1,8,25,53,91,75,61,17,51,81,21,86,45,68,55,77,38,16,10,11,6,76,78,18,49,29,96,92,88,99,3,66,41,15,58,82,28,36,71,24,97,4,79,90,70,5,54,74,73,65,64,87,47,85,69,32,23,57,13,67,35,62,9,50,60,31,30,95,20,63,22,94,26,14,19,59,42,72,98,12,0,43,2,39,89,56,44,80};
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
