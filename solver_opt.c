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
	double *C, *BB, *BtBt, *At, *AB, *ABAt;
	allocate_matrices(N, &C, &BB, &BtBt, &At, &AB, &ABAt);

	int i, j, k;

	// Compute BB = B * B
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = 0; k < N; ++k)
				BB[i * N + j] += B[i * N + k] * B[k * N + j];

	// Compute BtBt = (BB)t
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			BtBt[i * N + j] = BB[j * N + i];

	// Compute At
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			At[i * N + j] = A[j * N + i];

	// Compute AB = A * B
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = i; k < N; ++k)
				AB[i * N + j] += A[i * N + k] * B[k * N + j];

	// Compute ABAt = AB * At
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			for (k = j; k < N; ++k)
				ABAt[i * N + j] += AB[i * N + k] * At[k * N + j];

	// Compute C = ABAt + BtBt
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			C[i * N + j] = ABAt[i * N + j] + BtBt[i * N + j];

	free(BB);
	free(BtBt);
	free(At);
	free(AB);
	free(ABAt);

	return C;
}
