#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	int My_Rank,No_of_Process,root=0;
	int rows=3,columns=3;
	float A[9]={3,2,-4,2,3,3,5,-3,1},B[3]={3,15,14},X[3],localA[3],localB,modifiedA[3],modifiedB;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&No_of_Process);
	MPI_Comm_rank(MPI_COMM_WORLD,&My_Rank);

	if(No_of_Process!=rows)
	{
		if(My_Rank==root)
			printf("No of Process must be equal to %d !\n",rows);
		exit(0);
	}

	// scatter 1 row of A & B to each process
	MPI_Scatter(&A,columns,MPI_FLOAT,&localA,columns,MPI_FLOAT,root,MPI_COMM_WORLD);
	MPI_Scatter(&B,1,MPI_FLOAT,&localB,1,MPI_FLOAT,root,MPI_COMM_WORLD);

	// receive modified row from upper process to 
	for(int i=0;i<My_Rank;i++)
	{
		MPI_Recv(&modifiedA,3,MPI_FLOAT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(&modifiedB,1,MPI_FLOAT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		float fact=localA[i];
		for(int i=0;i<columns;i++)
			localA[i]-=fact*modifiedA[i];
		localB-=fact*modifiedB;
	}

	float pivote=localA[My_Rank];
	//exit if pivote zero
	assert(pivote!= 0);
	for(int i=My_Rank;i<columns;i++)
		localA[i]/=pivote;
	localB/=pivote;

	// send modified row to lowwer process Pi+1 to Pn
	for(int i=My_Rank+1;i<No_of_Process;i++)
	{
		MPI_Send(&localA,columns,MPI_FLOAT,i,My_Rank,MPI_COMM_WORLD);
		MPI_Send(&localB,1,MPI_FLOAT,i,My_Rank,MPI_COMM_WORLD);
	}

	printf("\n Final: At Rank %d\t\t",My_Rank);
	for(int j=0;j<columns;j++)
		printf("%f\t",localA[j]);
	printf("| %f\n\n",localB);

	/*.... Back Substitution starts from here ........*/
	for(int i=My_Rank+1;i<No_of_Process;i++)
	{
		float lowSol;
		MPI_Recv(&lowSol,1,MPI_FLOAT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		localB-=localA[i]*lowSol;
	}

	// send this solution to upper process
	for(int i=0;i<My_Rank;i++)
		MPI_Send(&localB,1,MPI_FLOAT,i,My_Rank,MPI_COMM_WORLD);

	// gather all sol from each process
	MPI_Gather(&localB,1,MPI_FLOAT,&X,1,MPI_FLOAT,root,MPI_COMM_WORLD);

	//display Soluction at root
	if(My_Rank==0)
	{
		printf("--------------------------------------------------- \n");
		printf("Solution vector \n");
		for(int irow = 0; irow < rows; irow++)
			printf("%.3lf\n",X[irow]);
		printf("--------------------------------------------------- \n");
	}

	MPI_Finalize();
	return 0;
}	
