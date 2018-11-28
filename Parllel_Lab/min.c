#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>


int main (int argc, char **argv)
	{
	  int a[12]={6,10,8,1,74,85,74,20,11,5,45,25},min,temp,i,j,locala[3];
	  int myrank,size,chunkSize,minimum;
	  
		MPI_Init (&argc,&argv);
		MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
		MPI_Comm_size(MPI_COMM_WORLD,&size);	
		
		chunkSize=12/size;
		int localb[chunkSize];
		MPI_Scatter(&a,chunkSize,MPI_INT,&locala,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
		
			min=a[0];
			
			for (i=0;i<chunkSize;i++)
				{
				  if (locala[i]<=min)
				  {
				  	min=locala[i];
				  }
				}
				
	
		MPI_Gather(&min,1,MPI_INT,&localb,1,MPI_INT,0,MPI_COMM_WORLD);	
		
			if (myrank==0)
			{

				minimum=localb[0];
				for (i=0;i<size;i++)
				{
					if (localb[i]<=minimum)
					{
						minimum=localb[i];
					}
				}
					printf ("%d\t",minimum);

			}
			
		MPI_Finalize();
	}
