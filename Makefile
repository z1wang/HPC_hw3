CXX=gcc-4.9

all: omp2 omp3 omp4 omp5 omp6 jacobi RBGS

omp2: omp_solved2.c
	$(CXX) -o omp2 omp_solved2.c -fopenmp

omp3: omp_solved3.c
	$(CXX) -o omp3 omp_solved3.c -fopenmp

omp4: omp_solved4.c
	$(CXX) -o omp4 omp_solved4.c -fopenmp

omp5: omp_solved5.c
	$(CXX) -o omp5 omp_solved5.c -fopenmp

omp6: omp_solved6.c
	$(CXX) -o omp6 omp_solved6.c -fopenmp	


jacobi: jacobi_omp.c	
	$(CXX) -o jacobi jacobi_omp.c -fopenmp

RBGS: RBGS_omp.c	
	$(CXX) -o RBGS RBGS_omp.c -fopenmp

clean:
	rm -rf *o omp2 omp3 omp4 omp5 omp6 jacobi RBGS