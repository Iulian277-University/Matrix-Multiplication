# Matrix Multiplication
## Taiatu Iulian-Marius - 332CB

## Problem statement
Write 3 implementations for the following matrix multiplication operation, where `A` is an `upper triangular` matrix and `B` is a `square` matrix:

$C = A * B * A' + B' * B'$

1. Unoptimized (`solver_opt.c`)
2. Optimized (`solver_opt.c`)
3. BLAS (`solver_blas.c`)

## Implementation details
### Unoptimized
The `unoptimized` version is a straightforward implementation of the matrix
multiplication algorithm. The only optimization is the fact that the `A` matrix is `upper triangular`, so I can skip the computations for the lower part of the matrix. In the same manner, I skip the computation fo the upper part of the `A'` matrix.

### Optimized
* One of the most important improves from the `unoptimized` version consists in the fact that I can precompute the `A'` and `B'` matrices (O(n^2)) and then use them in the computation of the result matrix.
In this way when I perform B * B, I do not traverse the second operand matrix B on `columns` (which is not `cache` friendly), but on `rows`.
Another interesting optimization is the property (A * B)' = B' * A'. This allows us to compute B * B and then transpose the entire matrix.

* Instead of using `indexes` of type `i * N + j`, I use `pointers` to the beginning of the row. This allows us to use the `row-major` order of the matrix, which is more `cache` friendly.

* I also use the `register` keyword in order to hint the compiler to store the variables in the `registers` instead of the `stack` to mitigate the `overhead` introduced by accesing the memory.

## Runtimes
### Unoptimized
```
Run=./tema2_neopt: N=400: Time=1.109967
Run=./tema2_neopt: N=600: Time=3.854163
Run=./tema2_neopt: N=800: Time=9.426256
Run=./tema2_neopt: N=1200: Time=29.527027
Run=./tema2_neopt: N=1600: Time=82.826088
```

### Optimized
```
Run=./tema2_opt_m: N=400: Time=0.215756
Run=./tema2_opt_m: N=600: Time=0.719241
Run=./tema2_opt_m: N=800: Time=1.665122
Run=./tema2_opt_m: N=1200: Time=5.622625
Run=./tema2_opt_m: N=1600: Time=14.283465
```

### BLAS
```
Run=./tema2_blas: N=400: Time=0.040032
Run=./tema2_blas: N=600: Time=0.131854
Run=./tema2_blas: N=800: Time=0.286257
Run=./tema2_blas: N=1200: Time=0.935947
Run=./tema2_blas: N=1600: Time=2.182930
```


