#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
    double startTime, endTime, totalTime;
    int myRank, commSize;
    
    for(int i=0; i < argc; ++i){   
        printf("Argument %d : %s\n", i, argv[i]);
    }

    MPI_Init(NULL, NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);

        if (myRank != 0) {
            int receiver = myRank + (myRank % 2 == 0 ? -1 : 1);

            startTime = MPI_Wtime();
            MPI_Send(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);
                    
            MPI_Recv(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            endTime = MPI_Wtime();
            totalTime = (double) (endTime - startTime);

            MPI_Send(&totalTime, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        	printf("From =>\t%d\tTo =>\t%d\tTime =>\t%f\n", myRank, receiver, totalTime);
        } else {
            double endTimeA = 0;
            double endTimeB = 0;
            double average = 0;
            int A, B;
            
            printf("-------------------------------------------------------------\n");
            for (A = 1, B = 2; A < commSize - 1; A += 2, B += 2) {
                MPI_Recv(&endTimeA, 1, MPI_DOUBLE, A, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&endTimeB, 1, MPI_DOUBLE, B, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                totalTime = endTimeA + endTimeB;
                average += totalTime;
                printf("Ping From =>\t%d\tTo =>\t%d\tTime =>\t%f\n", A, B, endTimeA);
                printf("Pong From =>\t%d\tTo =>\t%d\tTime =>\t%f\n", B, A, endTimeB);
                printf("Total Time =>\t%f\n", totalTime);
                printf("-------------------------------------------------------------\n");
            }
            
            average /= (commSize/ 2);
            printf("Ping-Pong Average =>%f\t", average);
        }
    
    MPI_Finalize();
return 0;
}
