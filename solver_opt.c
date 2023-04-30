/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

void allocate_matrices(int N,
						double **C,
						double **BB,
						double **BtBt,
						double **BAt,
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

	*BAt = calloc(N * N, sizeof(**BAt));
	if (*BAt == NULL)
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
	double *C, *BB, *BtBt, *BAt,*At, *Bt, *AB, *ABAt;
	allocate_matrices(N, &C, &BB, &BtBt, &BAt, &At, &Bt, &AB, &ABAt);
	
	////// Transpose A and B //////

	// Compute At and Bt
	for (register int i = 0; i < N; ++i) {
		register double *Aptr = A + i * N; // ith row of A
		register double *Bptr = B + i * N; // ith row of B

		register double *Atptr = At + i; // ith column of At
		register double *Btptr = Bt + i; // ith column of Bt

		for (register int j = 0; j < N; ++j) {
			*Atptr = *Aptr++;
			*Btptr = *Bptr++;
			Atptr += N;
			Btptr += N;
		}
	}

	////// B'B' //////

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

	// Compute BtBt = (BB)t
	for (register int i = 0; i < N; ++i) {
		register double *BBptr  = BB + i * N; // ith row of BB
		register double *BtBtptr = BtBt + i; // ith column of BBt

		for (register int j = 0; j < N; ++j) {
			*BtBtptr = *BBptr++;
			BtBtptr += N;
		}
	}

	////// ABA' //////

	// Compute AB = A * B
	for (register int i = 0; i < N; ++i) {
		register double *ABptr = AB + i * N; // ith row of AB
		register double *Acpy  = A  + i * N; // ith row of A

		for (register int j = 0; j < N; ++j, ++ABptr) {
			register double result = 0.0; // line of A * B

			register double *Aptr = Acpy + i;
			register double *Btptr = Bt + j * N + i; // jth row of Bt

			for (register int k = i; k < N; ++k, ++Aptr, ++Btptr)
				result += *Aptr * *Btptr;

			*ABptr = result;
		}
	}

	// Compute ABAt = AB * At
	// At = A traversed on rows
	for (register int i = 0; i < N; ++i) {
		register double *ABAtPtr = ABAt + i * N; // ith row of ABAt
		register double *ABptr = AB + i * N; // ith row of AB

		for (register int j = 0; j < N; ++j, ++ABAtPtr) {
			register double result = 0.0; // line of AB * At

			register double *ABptr2 = ABptr + j;
			register double *Atptr = A + j * (N + 1); // jth row of At

			for (register int k = j; k < N; ++k, ++ABptr2, ++Atptr)
				result += *ABptr2 * *Atptr;
			
			*ABAtPtr = result;
		}
	}

	// Compute C = ABAt + BtBt
	for (register int i = 0; i < N; ++i) {
		register double *Cptr = C + i * N; // ith row of C
		register double *ABAtPtr = ABAt + i * N; // ith row of ABAt
		register double *BtBtptr = BtBt + i * N; // ith row of BtBt

		for (register int j = 0; j < N; ++j, ++Cptr, ++ABAtPtr, ++BtBtptr)
			*Cptr = *ABAtPtr + *BtBtptr;
	}

	free(BB);
	free(BtBt);
	free(At);
	free(Bt);
	free(AB);
	free(ABAt);

	return C;
}
