#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double residue(int n, double* u){
  double res = 0;
  int s = n - 1;

  //unit is 1/h^2
  double unit = (n + 1) * (n + 1);  

  //temp is A * u - f
  double *temp = malloc(sizeof(double) * n);
  int i;
  for (i = 0; i < n; i++){
    temp[i] = 2 * u[i];
    if(i == 0){
      temp[i] = temp[i] - u[i + 1];
    }else if(i == s){
      temp[i] = temp[i] - u[i - 1];
    }else{
      temp[i] = temp[i] - u[i - 1] - u[i + 1];
    }
    temp[i] = temp[i] * unit;
    temp[i] = temp[i] - 1;
    res += temp[i] * temp[i];
  }
  free(temp);
  res = sqrt(res);
  printf("residue is %f\n", res);
  return res;
}


int main(int argc, char *argv[]){
	int i, j, nthreads, tid;
	double temp;

// N is the size of stencile, F is the size of iteration
	int N = atoi(argv[1]);
	if(N % 2 != 0){
		fprintf(stderr, "Size of stencile is not even\n");
    	abort();
  	}
  	int size = N / 2;
	int F = atoi(argv[2]);
	double unit = (N + 1) * (N + 1);

	double* red = calloc(size, sizeof(double));
	double* black = calloc(size, sizeof(double));
	double* rns = calloc(size, sizeof(double));
	double* bns = calloc(size, sizeof(double));
	double *t1;
	double *t2;
	double start = omp_get_wtime( );
	for(j = 1; j < F; j++){
		#pragma omp parallel for schedule(dynamic,10) private(temp, tid, t1) shared(rns, black)		
		for(i = 0; i < size; i++){
			if(i == 0){
				temp = 1 + black[0] * unit;
				rns[i] = temp / 2;
				rns[i] = rns[i]/unit;
			}else{
				temp = 1 + black[i - 1] * unit + black[i] * unit;
				rns[i] = temp / 2;
				rns[i] = rns[i]/unit;							
			}
		}
		t1 = red;
		red = rns;
		rns = t1;
		#pragma omp parallel for private(temp, tid, t2) shared(bns, red)		
		for(i = 0; i < size; i++){
			if(i == (size - 1)){
				temp = 1 + red[size - 1] * unit;
				bns[i] = temp / 2;
				bns[i] = bns[i]/unit;
			}else{
				temp = 1 + red[i] * unit + red[i + 1] * unit;
				bns[i] = temp / 2;
				bns[i] = bns[i]/unit;							
			}
		}
		t2 = black;
		black = bns;
		bns = t2;
	}
	double end = omp_get_wtime( );
	printf("Elapsed time is %f\n", end - start);
	double* res = calloc(N, sizeof(double));
	for(i = 0; i < size; i++){
		res[2 * i] = red[i];
		res[2 * i + 1] = black[i]; 
	}
	double r = residue(N, res);
	free(res);
	free(red);
	free(black);
	free(bns);
	free(rns);

}