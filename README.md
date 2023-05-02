# Matrix Multiplication
## Tăiatu Iulian-Marius - 332CB

## Problem statement
Write 3 implementations for the following matrix multiplication operation, where `A` is an `upper triangular` matrix and `B` is a `square` matrix:

$C = A * B * A' + B' * B'$, where $A'$ and $B'$ are the transposes of $A$ and $B$.

1. Unoptimized (`solver_neopt.c`)
2. Optimized (`solver_opt.c`)
3. BLAS (`solver_blas.c`)

## Implementation details
### Unoptimized
* The `unoptimized` version is a straightforward implementation of the matrix multiplication algorithm. The only optimization is the fact that the `A` matrix is `upper triangular`, so I can skip the computations for the lower part of the matrix. In the same manner, I skip the computation for the upper part of the `A'` matrix.

### Optimized
* One of the most important improves from the `unoptimized` version consists in the fact that I can precompute the `A'` and `B'` matrices (O(n^2)) and then use them in the computation of the result matrix.
In this way when I perform B * B, I do not traverse the second operand matrix B on `columns` (which is not `cache` friendly), but on `rows`.

* Another interesting optimization is the property (A * B)' = B' * A'. This allows us to compute B * B and then transpose the entire matrix.

* Instead of using `indexes` of type `i * N + j`, I use `pointers` to the beginning of the row. This allows us to use the `row-major` order of the matrix, which is more `cache` friendly.

* I also use the `register` keyword in order to hint the compiler to store the variables in the `registers` instead of the `stack` to mitigate the `overhead` introduced by accesing the memory.

### BLAS
* I have used `cblas_dtrmm()` to multiply a triangular matrix with a square matrix and `cblas_dgemm()` to multiply two square matrices.
In the end, I have compute a manual sum of the two result matrices (O(n^2)), because I have not found a BLAS function to do the sum of two matrices. 

## Runtimes
Based on the following `runtimes`, we can observe that the optimized implementation performs significantly better than the unoptimized implementation, with a `speedup` of ~5x.

Compared to the optimized implementation, the `BLAS` program is faster by ~40 for large matrices (N=1600).

Overall, it appears that the optimized implementation brings a good improvement in performance, but it is still far from the performance of the `BLAS` implementation.

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

## Cachegrind
From the following cachegrind results, we can see that the `optimized` version has a better `cache` performance than the `unoptimized` version. This is due to the fact that the `optimized` version uses the `row-major` order of the matrix, which is more `cache` friendly.

However, the number of branches for the `optimized` and `unoptimized` versions is almost the same. This suggests that the optimization did not have much impact on the `branch prediction`.

Comparing the `BLAS` version with the `optimized` version, we can see that the number of `instructions` and `data` references is much smaller. This is due to the fact that the `BLAS` version uses `vectorization` and is highly optimized for cache efficiency and is able to make better use of the hardware resources.

### Unoptimized
```
I   refs:      5,924,932,875
D   refs:      2,962,909,328
Branches:        132,240,834
```

### Optimized
```
I   refs:      1,542,332,468
D   refs:        398,436,067
Branches:        132,562,757
```

### BLAS
```
I   refs:      256,352,144
D   refs:       96,597,644
Branches:        4,709,046
```

I refs = Instructions References, D refs = Data References

## Conclusion
In conclusion, the `optimized` version is much faster than the `unoptimized` version, but it is still far from the performance of the `BLAS` version. This is due to the fact that the `BLAS` version uses `vectorization` and is also highly optimized for cache efficiency and is able to make better use of the hardware resources.
