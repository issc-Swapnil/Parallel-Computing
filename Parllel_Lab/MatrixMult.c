#include<mpi.h>
#include<stdio.h>

int main(int argc,char **argv)
{
	int row=4,col=4;
	float mat[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},vec[4]={1,1,1,1},recv_buf[4];
	int res[4];
	
	MPI_Init(&argc,&argv);
	int size,rank;
	int start_point=2;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	// row/size*col
	
	MPI_Bcast(&vec,4,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(&mat,4,MPI_FLOAT,&recv_buf,4,MPI_FLOAT,0,MPI_COMM_WORLD);
	
	/*printf("Rank is %d \n",rank);
	for(int i=0;i<4;i++)
		printf("ele %f\t",recv_buf[i]);
	printf("\n");*/
	
	// multiply
	float mult=0;
	for(int i=0;i<4;i++)
		mult+=recv_buf[i]*vec[i];
	//printf("mult:%f\n",mult);	
	if(rank==0){
		
		float tp=0,final_res[4];
		final_res[0]=mult;
		for(int j=1;j<size;j++){
			MPI_Recv(&tp,1,MPI_FLOAT,j,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			final_res[j]=tp;
		}
		
		printf("Multiplication is \n");
		for(int j=0;j<4;j++){
			printf("%f\n",final_res[j]);
		}
		
	}
	else{
		MPI_Send(&mult,1,MPI_INT,0,0,MPI_COMM_WORLD);
	}
	
	
	MPI_Finalize();
	
}
