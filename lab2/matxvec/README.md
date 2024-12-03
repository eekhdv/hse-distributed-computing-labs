# Multiplying a matrix by a vector 

## General

### How to build
```sh
make
```

### How to run 
```sh
cd ./bin
mpiexec -n <N> ./matxvec.o <rows> <cols>
# <N>    - The number of processes.
# <rows> - The number of rows.
# <cols> - The number of columns.
```

## Tests

### How to build tests
```sh
make test
```

### How to run test
```sh
cd ./bin
mpiexec -n <N> ./matxvec.o # <N> - Specify the number of processes.
```
