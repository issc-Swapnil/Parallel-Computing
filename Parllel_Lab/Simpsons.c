#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
#include<math.h>
double function (double f);

int main(int argc , char **argv)
{
	int size,rank;
	double a=0,b=1,h,sum=0,G_sum=0;


	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	h=(b-a)/size;

	a=rank*h;
	b=a+h;

	sum = function(a)+3*function((2*a+b)/3)+3*function(a+(2*b)/3)+function(b);
	sum = ((b-a)/8)*sum;
	sleep(rank);

	printf("\nSum is: %lf\n",sum);
	

	MPI_Reduce(&sum,&G_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank==0)
		printf("\nFinal Sum is := %lf\n",G_sum);

	MPI_Finalize();
}
double function (double f)
{
	return 2+sin(2*sqrt(f));
}
