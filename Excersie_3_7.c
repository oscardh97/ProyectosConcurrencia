#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]) {
    double startTime, endTime, totalTime;//Variables para manejo de tiempo usando MPI_Wtime
    clock_t startTimeClock, endTimeClock;//Variables para manejo de tiempo usando clock_t
    int myRank, commSize;//Variables para control de MPI

    int dummyIterations = 1000000;//Veces para repetir un ciclo vacío
    
    /*
    * Variable para controlar el tipo de ejecución. 
    * Se envía como el segundo parámetro desde la consola, luego del nombre del ejecutable.
    * Si es 0 se utilizará clock_t
    * Si es 1 se utilizará MPI_Wtime
    */
    int type = argc >= 2 ? atoi(argv[1]) : 1;

    /*
    * En caso de querer usar clock_t se puede controlar los ciclos del for vacío.
    * Se envía como tercer parámetro luego del tipo de ejecución
    */
    if (type == 0) {
        if (argc == 3) {
            dummyIterations = atoi(argv[2]);
        }

    }

    MPI_Init(NULL, NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);

        /*
        * Si el proceso no es el cero, se realiza el ping-pong
        */
        if (myRank != 0) {
            /*
            * El receptor será el proceso siguiente proceso o el anterior proceso
            */
            int receiver = myRank + (myRank % 2 == 0 ? -1 : 1); 

            if (type == 0) {// Caso que se utilize el clock de C
                startTimeClock = clock();//INICIO DE TIEMPO DE ENVÍO

                //DUMMY FOR
                for (int i = 0; i < dummyIterations; ++i) {
                    //DUMMY LINE
                }

                //PING---------------------------------------------------------->>>>
                MPI_Send(&startTimeClock, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);

                //<<<-----------------------------------------------------------PONG
                MPI_Recv(&startTimeClock, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                endTimeClock = clock();// FIN TIEMPO DE RECIBIDO
                totalTime = difftime(endTimeClock, startTimeClock) / CLOCKS_PER_SEC;//DIFERENCIA TIEMPO
            } else {// Caso que se utilize MPI_Wtime
                startTime = MPI_Wtime();//INICIO DE TIEMPO DE ENVÍO

                //PING---------------------------------------------------------->>>>
                MPI_Send(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD);

                //<<<-----------------------------------------------------------PONG
                MPI_Recv(&startTime, 1, MPI_DOUBLE, receiver, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                endTime = MPI_Wtime();// FIN TIEMPO DE RECIBIDO
                totalTime = endTime - startTime;//DIFERENCIA TIEMPO 
            }

            /*
            * Enviar cada tiempo que tomó cada ping y cada pong a un proceso master
            */
            MPI_Send(&totalTime, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

            // printf("From =>\t%d\tTo =>\t%d\tTime =>\t%f\n", myRank, receiver, totalTime);
        } else {
            double endTimeA = 0;//Tiempo que tomó de A hacia B
            double endTimeB = 0;//Tiempo que tomó de B hacia A
            double average = 0;//Promedio de todos los Ping-Pong
            int A, B;//Procesos emisores

            printf("The time will be calculated using %s\n", type == 1 ? "MPI_Wtime" : "CLOCK C");
            printf("The program will be executed %d times a dummy line\n", dummyIterations);
            printf("-------------------------------------------------------------\n");

            // El master process recibe los tiempos individuales y suma el ping con su pong correspondiente
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
            printf("Ping-Pong Average =>\t%f\n", average);
        }
    
    MPI_Finalize();
    return 0;
}
