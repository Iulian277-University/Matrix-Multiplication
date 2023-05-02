/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include <stdlib.h>
#include "utils.h"

void allocate_matrices(int N,
						double **C,
						double **AB,
						double **ABAt,
						double **BtBt) {

	*C = calloc(N * N, sizeof(**C));
	if (*C == NULL)
		exit(EXIT_FAILURE);

	*AB = calloc(N * N, sizeof(**AB));
	if (*AB == NULL)
		exit(EXIT_FAILURE);

	*ABAt = calloc(N * N, sizeof(**ABAt));
	if (*ABAt == NULL)
		exit(EXIT_FAILURE);

	*BtBt = calloc(N * N, sizeof(**BtBt));
	if (*BtBt == NULL)
		exit(EXIT_FAILURE);
}

/*
 * Add your unoptimized implementation here
 */
double* my_solver(int N, double *A, double* B) {
	double *C, *AB, *ABAt, *BtBt;
	allocate_matrices(N, &C, &AB, &ABAt, &BtBt);

	int i, j, k;

	// Compute AB = A * B
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = i; k < N; ++k)
				AB[i * N + j] += A[i * N + k] * B[k * N + j];

	// Compute ABAt = AB * A'
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = j; k < N; ++k)
				ABAt[i * N + j] += AB[i * N + k] * A[j * N + k];
	
	// Compute BtBt = B' * B'
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = 0; k < N; ++k)
				BtBt[i * N + j] += B[k * N + i] * B[j * N + k];

	// Compute C = ABAt + BtBt
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			C[i * N + j] = ABAt[i * N + j] + BtBt[i * N + j];

	free(AB);
	free(ABAt);
	free(BtBt);

	return C;
}
