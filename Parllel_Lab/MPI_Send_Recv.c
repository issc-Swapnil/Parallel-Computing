#include<stdio.h>
#include<mpi.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	int size, rank,i,start,end,chunksize=0,sum=0,n=100,finalSum=0;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	chunksize= n/size;
	start= chunksize*rank;
	end = start+chunksize;	

	for(i=start+1;i<=end;i++)
	{
		sum+=i;
	}
	printf("Sum= %d\n",sum);



	if (rank) 
	{
		MPI_Send(&sum,1,MPI_INT,0,0, MPI_COMM_WORLD);
 	}
	else
	{
		int j=0;
		for(j=1;j<4;j++)
		{
			MPI_Recv(&finalSum,1,MPI_INT,j,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			sum+=finalSum;
		}
		printf("\nFinal Sum = %d\n",sum);
	}
	
	MPI_Finalize();
}
