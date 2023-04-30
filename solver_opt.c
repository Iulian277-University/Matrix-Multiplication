/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

void allocate_matrices(int N,
						double **C,
						double **BB,
						double **BtBt,
						double **At,
						double **Bt,
						double **AB,
						double **ABAt) {
	
	*C = calloc(N * N, sizeof(**C));
	if (*C == NULL)
		exit(EXIT_FAILURE);

	*BB = calloc(N * N, sizeof(**BB));
	if (*BB == NULL)
		exit(EXIT_FAILURE);

	*BtBt = calloc(N * N, sizeof(**BtBt));
	if (*BtBt == NULL)
		exit(EXIT_FAILURE);

	*At = calloc(N * N, sizeof(**At));
	if (*At == NULL)
		exit(EXIT_FAILURE);

	*Bt = calloc(N * N, sizeof(**Bt));
	if (*Bt == NULL)
		exit(EXIT_FAILURE);
	
	*AB = calloc(N * N, sizeof(**AB));
	if (*AB == NULL)
		exit(EXIT_FAILURE);
	
	*ABAt = calloc(N * N, sizeof(**ABAt));
	if (*ABAt == NULL)
		exit(EXIT_FAILURE);
}

/*
 * Add your optimized implementation here
 * C = A * B * A' + B' * B'
 */
double* my_solver(int N, double *A, double* B) {
	double *C, *BB, *BtBt, *At, *Bt, *AB, *ABAt;
	allocate_matrices(N, &C, &BB, &BtBt, &At, &Bt, &AB, &ABAt);

	// Compute At and Bt
	for (register int i = 0; i != N; ++i) {
		register double *Aptr = A + i * N; // ith row of A
		register double *Bptr = B + i * N; // ith row of B

		register double *Atptr = At + i; // ith column of At
		register double *Btptr = Bt + i; // ith column of Bt

		for (register int j = 0; j != N; ++j) {
			*Atptr = *Aptr++;
			*Btptr = *Bptr++;
			Atptr += N;
			Btptr += N;
		}
	}


	// Compute B * At (store in C for now)
	for (register int i = 0; i != N; ++i) {
		register double *Cptr = C + i * N; // ith row of C
		register double *Bcpy = B + i * N; // ith row of B

		for (register int j = 0; j != N; ++j, ++Cptr) {
			register double result = 0.0; // line of B * At

			register double *Bptr = Bcpy + j;
			register double *Aptr = A + j * (N + 1);

			for (register int k = j; k < N; ++k, ++Bptr, ++Aptr)
				result += *Bptr * *Aptr;

			*Cptr = result;
		}
	}

	// C for now is B * At

	// Compute BB = B * B; I will use Bt traversed on rows
	for (register int i = 0; i < N; ++i) {
		register double *BBptr = BB + i * N; // ith row of BB
		register double *Bcpy  = B  + i * N; // ith row of B

		for (register int j = 0; j < N; ++j, ++BBptr) {
			register double result = 0.0; // line of B * B

			register double *Bptr  = Bcpy;
			register double *Btptr = Bt + j * N; // jth row of Bt

			for (register int k = 0; k < N; ++k, ++Bptr, ++Btptr)
				result += *Bptr * *Btptr;
			
			*BBptr = result;
		}
	}

	int i, j, k;
	// Compute BB = B * B
	// for (i = 0; i < N; ++i)
	// 	for (k = 0; k < N; ++k)
	// 		for (j = 0; j < N; ++j)
	// 			BB[i * N + j] += B[i * N + k] * B[k * N + j];

	// Compute BtBt = (BB)t
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			BtBt[i * N + j] = BB[j * N + i];

	// Compute AB = A * B
	// for (i = 0; i < N; ++i)
	// 	for (j = 0; j < N; ++j)
	// 		for (k = i; k < N; ++k)
	// 			AB[i * N + j] += A[i * N + k] * B[k * N + j];

	// Compute ABAt = A * BAt = A * C
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = i; k < N; ++k)
				ABAt[i * N + j] += A[i * N + k] * C[k * N + j];

	// Compute C = ABAt + BtBt
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			C[i * N + j] = ABAt[i * N + j] + BtBt[i * N + j];

	free(BB);
	free(BtBt);
	free(At);
	free(Bt);
	free(AB);
	free(ABAt);

	return C;
}
