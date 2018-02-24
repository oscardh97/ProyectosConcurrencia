#include <stdio.h>
#include <string.h>
#include <mpi.h>

double f(double x) {
        return x;
}

double Rectangle(double leftEndpt, double rightEndpt, int rectCount, double baseLen) {
        printf("%d%d\n", leftEndpt, rightEndpt);
        double estimate, x;
        int i;

        estimate = f(leftEndpt);
        for (int i = 1; i <= rectCount - 1; ++i) {
                x = leftEndpt + i * baseLen;
                estimate += f(x);
        }

        estimate = estimate * baseLen;

        return estimate;
}

int main(int argc, char const *argv[]) {
        int myRank, commSize, n = 1024, localN;
        double a = 0.0, b = 5000.0, h, localA, localB;
        double localInt, totalInt;
        int source;

        MPI_Init(NULL, NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);


        h = (b-a) / n;
        localN = n /commSize;

        localA = a + myRank * localN * h;
        localB = localA + localN * h;
        localInt = Rectangle(localA, localB, localN, h);

        if (myRank != 0) {
                MPI_Send(&localInt, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
                totalInt = localInt;
                for (source = 1; source < commSize; source++) {
                        MPI_Recv(&localInt, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        totalInt += localInt;
                }

                printf("With n = %d trapezoids, out estimat\n", n);
                printf("of the integral from %f to %f = %1.5e\n",a, b, totalInt);
        }

        MPI_Finalize();
        return 0;
}
                        
