/*
	mpicc -o Pi_Calculation Pi_Computation.c -lm
	mpirun -np 4 Pi_Calculation

*/

#include<stdio.h>
#include<mpi.h>
#include<time.h>
#include<math.h>
#include <stdlib.h>

int main (int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int size,myrank;
	unsigned long int i,total_random_no_genrated= 10000000;
	float Pi=0;
	
	MPI_Comm_size(MPI_COMM_WORLD,&size);   		// size is declared 
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank); 		// rank is declared	

	unsigned long int random_numbers=total_random_no_genrated/size;
	
	//setting a seed using current time 
	srand(myrank*time(NULL));	 
  	unsigned long int no_of_points_inside_circle=0;

	for (i=0;i<random_numbers;i++)
	{
	    float x= (float) rand()/RAND_MAX;               			
		float y= (float) rand()/RAND_MAX; 
		// X^2+Y^2<=R^2 here R=0.5             
		if ((pow(x,2)+pow(y,2))<=0.25)
		    no_of_points_inside_circle++;	  
	}
		
	float local_pi=(4.0*no_of_points_inside_circle)/random_numbers;
    printf ("Local Pi=%f\n",local_pi);
	
	// send local_pi value to Root process
    MPI_Reduce(&local_pi,&Pi,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
                
	
	if(myrank==0)
	{
		// find error in value
		float error=fabs(Pi-M_PI);
        printf("\n\nCalculated value of Pi is %f  with %f error.\n", Pi,error);
    }
       	           	            
	MPI_Finalize();
}
