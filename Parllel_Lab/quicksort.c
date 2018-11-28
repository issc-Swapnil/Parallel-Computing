#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(int argc, char ** argv)
{
	int A[] = {15,8,12,7,25,17,22,5};
	int final[4][16], num[4];
	int chunk = sizeof(A)/sizeof(int);
	int i, j, k, pivot, size, rank, new[chunk], temp, n;
	int l[chunk], r[chunk], a[chunk], nw[chunk];

	MPI_Init(&argc, &argv);      
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	chunk = chunk/size;
	//MPI_Scatter( void* send_data, int send_count,MPI_Datatype send_datatype,void* recv_data,int recv_count,MPI_Datatype           recv_datatype,int root,MPI_Comm communicator)
    MPI_Scatter(&A, chunk, MPI_INT, &a, chunk, MPI_INT, 0, MPI_COMM_WORLD);
	pivot = A[0];
	//int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root,MPI_Comm comm )
	MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (i = 0, j = 0, k = 0; i < chunk; i++)
	{
       	if (a[i] <= pivot)
        {
            l[j] = a[i];
            j++;
        }
        else
        {
            r[k] = a[i];
            k++;
        }
	}
	
	if(rank >= (size/2))
	{
		temp = (rank-(size/2));
		MPI_Send(&j, 1, MPI_INT, temp, 0, MPI_COMM_WORLD);
	   	MPI_Send(&l, j, MPI_INT, temp, 0, MPI_COMM_WORLD);
		MPI_Recv(&n, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	MPI_Recv(&nw, n, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	
	   	for(i = 0; i < k; i++)
			new[i] = r[i];
		for(j = 0; j < n; i++, j++)
			new[i] = nw[j];
		n=n+k;
	}
	else
	{
		temp = (rank+(size/2));
		MPI_Recv(&n, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	MPI_Recv(&nw, n, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&k, 1, MPI_INT, temp, 0, MPI_COMM_WORLD);
	   	MPI_Send(&r, k, MPI_INT, temp, 0, MPI_COMM_WORLD);
		for(i = 0; i < j; i++)
			new[i] = l[i];
		for(k = 0; k < n; i++, k++)
			new[i] = nw[k];
		n=n+j;
	}
	if(rank == 0 || rank == 2)
	{
		pivot = new[0];
		temp = rank + 1;
		MPI_Send(&pivot, 1, MPI_INT, temp, 0, MPI_COMM_WORLD);
	}
	else
	{
		temp = rank - 1;
		MPI_Recv(&pivot, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	for (i = 0, j = 0, k = 0; i < n; i++)
	{
	       	if (new[i] <= pivot)
	        {
	            l[j] = new[i];
	            j++;
	        }
	        else
	        {
	            r[k] = new[i];
	            k++;
	        }
	}
	if(rank%2)
	{
		temp = rank - 1;
		MPI_Send(&j, 1, MPI_INT, temp, 0, MPI_COMM_WORLD);
	   	MPI_Send(&l, j, MPI_INT, temp, 0, MPI_COMM_WORLD);
		MPI_Recv(&n, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	MPI_Recv(&nw, n, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	for(i = 0; i < k; i++)
			new[i] = r[i];
		for(j = 0; j < n; i++, j++)
			new[i] = nw[j];
		n=n+k;
	}
	else
	{
		temp = rank + 1;
		MPI_Recv(&n, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   	MPI_Recv(&nw, n, MPI_INT, temp, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&k, 1, MPI_INT, temp, 0, MPI_COMM_WORLD);
	   	MPI_Send(&r, k, MPI_INT, temp, 0, MPI_COMM_WORLD);
		for(i = 0; i < j; i++)
			new[i] = l[i];
		for(k = 0; k < n; i++, k++)
			new[i] = nw[k];
		n=n+j;
	}
	for (int i = 0; i < n-1; ++i)
	{
		for (int j = 0; j < n-1; ++j)
		{
			if (new[j] > new[j+1])
			{
				temp = new[j];
				new[j] = new[j+1];
				new[j+1] = temp;
			}
		}
	}
	MPI_Gather(&n, 1, MPI_INT,&num, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (rank)
	{
		MPI_Send(&new, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	if (!rank)
	{
		for (int i = 0; i < num[0]; ++i)
			final[0][i] = new[i];
		for(i = 1; i < 4; i++)
			MPI_Recv(&final[i], num[i], MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < num[i]; ++j)
				printf(" %d ", final[i][j]);
	}

	printf("\n");
	MPI_Finalize();
}
