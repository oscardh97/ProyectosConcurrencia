#include <stdio.h>
#include <omp.h>
#include <math.h>  
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	#pragma omp parallel for num_threads(thread_count)
	int i;
	double mySum = 0.0;
	for (i = 0; i < n; i++) {
		#pragma omp atomic
		mySum += sin(i);
	}

	return 0;
}