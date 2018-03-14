#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void Hello(void);
int main(int argc, char* argv[]) {
	int thread count = strtol(argv[1], NULL, 10);

	# pragma omp parallel num threads(thread count)
	Hello();
	return 0;
}

void Hello(void) {
	int my rank = omp get thread num();

	int thread count = omp get num threads();
	printf("Hello from thread %d of %dnn", my rank, thread count);
}