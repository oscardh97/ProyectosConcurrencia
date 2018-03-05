#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

void* Hello(void* rank);

int main(int argc, char const *argv[]) {
	long thread;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc(thread_count * sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}

	free(thread_handles);

	return 0;
}

void* Hello(void* rank) {
	printf("My rank is %ld of %d\n", (long)rank, thread_count);

	return NULL;
}
