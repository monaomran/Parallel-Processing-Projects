#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char * argv[])
{
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/

    int globalSum = 0;
    double sumOfDiff=0;
    int n;
    int localsum = 0;
    double meanOfNumber=0;
    int* arr;
    arr =  malloc (10 * sizeof (int));

    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc, &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if( my_rank == 0)
    {
        printf("Enter your number: ");
        scanf("%d", &n);


    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(my_rank!=0)
    {
        int i;
        int temp;   //to store the random number temporarily

        srand(time(0));
        localsum=0;
        for(i=0; i<n ; i++)
        {
            temp=rand()%10;
            localsum+=temp;
            //printf("localsum : %d \n",localsum);
            arr[i]=temp;
        }
    }
    MPI_Allreduce(&localsum,&globalSum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    if(my_rank!=0)
    {
        int i;
        double mean;

       // printf("globalsum : %d \n",globalSum);
        mean=(double)globalSum/(n * (p-1));
        printf("Numbers of process %d \n",my_rank);
        for(i=0; i<n ; i++)
        {
            printf("%d ,\n",arr[i]);
            meanOfNumber+=((arr[i]-mean)*(arr[i]-mean));

        }

    }
    MPI_Reduce(&meanOfNumber,&sumOfDiff,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    if (my_rank == 0 )
    {
        double root=(double)sumOfDiff/(n*(p-1));
        root=sqrt(root);
        printf("Standard deviation: %lf",root);
    }
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
