/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include "utils.h"

void allocate_matrices(int N,
						double **C,
						double **BtBt) {

	*C = calloc(N * N, sizeof(**C));
	if (*C == NULL)
		exit(EXIT_FAILURE);

	*BtBt = calloc(N * N, sizeof(**BtBt));
	if (*BtBt == NULL)
		exit(EXIT_FAILURE);
}

/* 
 * Add your BLAS implementation here
 * C = A * B * A' + B' * B'
 */
double* my_solver(int N, double *A, double *B) {
	double *C, *BtBt;
	allocate_matrices(N, &C, &BtBt);

	// Compute C = A * B
	memcpy(C, B, N * N * sizeof(*C));
	// C <- A * C
	cblas_dtrmm(
		CblasRowMajor,
		CblasLeft,
		CblasUpper,
		CblasNoTrans,
		CblasNonUnit,
		N,
		N,
		1.0,
		A,
		N,
		C,
		N
	);

	// Compute C = C * A'
	cblas_dtrmm(
		CblasRowMajor,
		CblasRight,
		CblasUpper,
		CblasTrans,
		CblasNonUnit,
		N,
		N,
		1.0,
		A,
		N,
		C,
		N
	);

	// Compute B' * B'
	memcpy(BtBt, B, N * N * sizeof(*BtBt));
	cblas_dgemm(
		CblasRowMajor,
		CblasTrans,
		CblasTrans,
		N,
		N,
		N,
		1.0,
		B,
		N,
		BtBt,
		N,
		0.0,
		BtBt,
		N
	);

	// Compute C = C + BtBt
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			C[i * N + j] += BtBt[i * N + j];

	// Free memory
	free(BtBt);

	return C;
}
