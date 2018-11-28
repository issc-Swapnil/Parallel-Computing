#include <mpi.h>
#include <stdio.h>
#include <math.h>
/*
	$ mpicc simpson_rule.c -lm
	$ mpirun -np 5 a.out
*/

/*
	Simpson's 3/8 rule
	integration of f(x) from a to b=(b-a)/8*[f(a)+3*f((2a+b)/3+3*f((a+2b)/3)+f(b)]
*/

double cal_Fx(double x)
{
	double ans=0;
	// f(x)=2+ sin(2*sqrtx))
	ans=2+sin(2*sqrt(x));
	return ans;
}

double cal_simpson_value(double a,double b)
{
	double sol=0;
	sol=(b-a)/8 * (cal_Fx(a)+3*cal_Fx((2*a+b)/3)+3*cal_Fx((a+2*b)/3)+cal_Fx(b));
	return sol;	
}

int main(int argc,char **argv)
{
	int size,rank;
	double integralFx=0,a=0,b=1;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD ,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // divide interval in subdomains of the range from [a: b]
    double interval_size=(b-a)/size;
    double leftLimit=rank*interval_size;
    double rightLimit=leftLimit+interval_size;
    double partial_integral=cal_simpson_value(leftLimit,rightLimit);

   	printf("At Rank %d\tPartial Integral of Fx from %lf to %lf is %lf.\n",rank,leftLimit,rightLimit,partial_integral);

   	MPI_Reduce(&partial_integral,&integralFx, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

	if(rank==0){
    	printf("\n\nIntegral of Fx is %lf.\n",integralFx);
    }

    MPI_Finalize();
}