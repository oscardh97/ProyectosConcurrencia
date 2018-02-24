#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
   clock_t start, end, total;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    double average = 0;
    int count = 0;
    // char label[100] = "Hi I'm the processor ";

    if (myRank != 0) {
    	count++;
        MPI_Send(count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int source = 1; source < commSize; source++) {
            MPI_Recv(count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    MPI_Finalize();

	return 0;
}