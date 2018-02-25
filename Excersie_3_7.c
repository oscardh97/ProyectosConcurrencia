#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
    double startTime, endTime, totalTime;
    int myRank, commSize;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    double average = 0;
    int count = 0;

    int receiver = myRank + (myRank % 2 == 0 ? 1 : -1);

    startTime = MPI_Wtime();
    MPI_Send(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);
    
    MPI_Recv(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    endTime = MPI_Wtime();

    totalTime = (double) (endTime - startTime);

	printf("From =>\t%d\tTo =>\t%d\tTime =>\t%f\n", myRank, receiver, totalTime);
    
    // for (int source = 0; source < commSize; source++) {
    //     MPI_Recv(count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }

    MPI_Finalize();

	return 0;
}
