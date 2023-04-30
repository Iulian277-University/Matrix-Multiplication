#!/bin/bash

### Tests ###

make clean && make
./tema2_neopt /export/asc/tema2/input
./compare out1 /export/asc/tema2/out1 0.0001
./compare out2 /export/asc/tema2/out2 0.0001
./compare out3 /export/asc/tema2/out3 0.0001


make clean && make
./tema2_opt_m /export/asc/tema2/input
./compare out1 /export/asc/tema2/out1 0.0001
./compare out2 /export/asc/tema2/out2 0.0001
./compare out3 /export/asc/tema2/out3 0.0001

# make clean && make
# ./tema2_opt_blas /export/asc/tema2/input
# ./compare out1 /export/asc/tema2/out1 0.0001
# ./compare out2 /export/asc/tema2/out2 0.0001
# ./compare out3 /export/asc/tema2/out3 0.0001


### Valgrind ###
make clean && make
valgrind --tool=memcheck --leak-check=full --log-file="neopt.memory" ./tema2_neopt /export/asc/tema2/input_valgrind
valgrind --tool=memcheck --leak-check=full --log-file="opt_m.memory" ./tema2_opt_m /export/asc/tema2/input_valgrind
# valgrind --tool=memcheck --leak-check=full --log-file="opt_blas.memory" ./tema2_blas /export/asc/tema2/input_valgrind
