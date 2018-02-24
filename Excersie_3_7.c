#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
    clock_t startTime, endTime, totalTime;
    int myRank, commSize;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    double average = 0;
    int count = 0;
    // char label[100] = "Hi I'm the processor ";

    int receiver = myRank + (myRank % 2 == 0 ? 1 : -1);
	printf("My Rank =>\t%d\nReceiver =>\t%d\n", myRank, receiver);

    startTime = clock();
    MPI_Send(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);
    
    MPI_Recv(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    endTime = clock();

    totalTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;

    printf("Total Time => \t%f\n", totalTime);

    // if (myRank != 0) {
    // 	count++;
    //     MPI_Send(count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // } else {
    //     for (int source = 1; source < commSize; source++) {
    //         MPI_Recv(count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     }
    // }

    MPI_Finalize();

	return 0;
}
