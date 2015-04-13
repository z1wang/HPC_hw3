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
	int F = atoi(argv[2]);
	double unit = (N + 1) * (N + 1);


	double* res = calloc(N, sizeof(double));
	double* ans = calloc(N, sizeof(double));
	double *t;
	double start = omp_get_wtime( );
	for(j = 1; j < F; j++){		
		#pragma omp parallel for private(temp, tid, i) shared(ans, res)		
		for(i = 0; i < N; i++){
			if(i == 0){
				temp = 1 + res[i + 1] * unit;
				ans[i] = temp / 2;
				ans[i] = ans[i]/unit;
			}else if(i == N - 1){
				temp = 1 + res[i - 1] * unit;
				ans[i] = temp / 2;
				ans[i] = ans[i]/unit;					
			}else{
				temp = 1 + res[i + 1] * unit + res[i - 1] * unit;
				ans[i] = temp / 2;
				ans[i] = ans[i]/unit;							
			}
		}
		t = res;
		res = ans;
		ans = t;
	}
	double end = omp_get_wtime( );
	printf("Elapsed time is %f\n", end - start);
	double r = residue(N, res);
	free(res);
	free(ans);
}