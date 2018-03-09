#include <stdio.h>
#include <string.h>
#include <mpi.h>

double f(double x) {
        return x;
}
double totalInt = 0;
void* Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen);

int main(int argc, char const *argv[]) {
        int myRank, commSize, n = 1024, localN;
        double a = 0.0, b = 5000.0, h, localA, localB;
        double localInt;
        int source;

        h = (b-a) / n;
        localN = n /commSize;

        localA = a + myRank * localN * h;
        localB = localA + localN * h;
        localInt = Trap(localA, localB, localN, h);

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

        return 0;
}
                        
void* Trap(double leftEndpt, double rightEndpt, int trapCount, double baseLen) {
        printf("%d%d\n", leftEndpt, rightEndpt);
        double estimate, x;
        int i;

        estimate = (f(leftEndpt) + f(rightEndpt)) / 2;
        for (int i = 1; i <= trapCount - 1; ++i) {
                x = leftEndpt + i * baseLen;
                estimate += f(x);
        }

        totalInt += estimate * baseLen;

        return NULL;
}
