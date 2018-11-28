#include<stdio.h>
#include<mpi.h>
#include<math.h>

int main (int argc,char **argv)
{
	int array[]={1,2,3,4,5,6,7,8,9,10,11,12},sum=0;
	int i,size,rank,chunkSize,sumofAll;
	int chunk;
	
	MPI_Init (&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
 	chunk = sizeof(array)/sizeof(int);
	chunkSize = chunk/size;
	int a[chunkSize];
	
//	printf("\n %d",chunkSize);
	 	
	MPI_Scatter (&array,chunkSize,MPI_INT,&a,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
	
	for (i=0;i<chunkSize;i++)
		{
			sum=sum+(pow(a[i],2));

		}

    	MPI_Reduce(&sum,&sumofAll,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

//				printf ("==>%d==>\n",sum);	
//	MPI_Gather(&sum,1,MPI_INT,&sumofAll,1,MPI_INT,0,MPI_COMM_WORLD);

					
				if (rank==0)
				{
//					for (i=0;i<size;i++)
						//{
							printf ("%d\n",sumofAll);
					        //}
				}
	
	MPI_Finalize();
}
