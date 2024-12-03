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

## Stats and Results

| division by columns | division by rows |
| ------------------- | ---------------- |
| ![division by columns  20000x100000 x2000](https://github.com/user-attachments/assets/87bb5ee7-86f2-42cc-8fa3-cad4f24b5ef8) | ![division by row  2000 100000 x2000](https://github.com/user-attachments/assets/88656af5-cdd0-4153-b3d6-af24ede9107e) |
| ![division by columns 2000x 20000 100000](https://github.com/user-attachments/assets/760d4a34-1d08-4f12-9ac4-47207738dac5) | ![division by row 2000x 20000 100000](https://github.com/user-attachments/assets/ba13db45-6a34-4124-b0ae-0b81e0d45e01) |

| block division |
| -------------- |
| ![Block division 4 proc  20000 100000 x2000](https://github.com/user-attachments/assets/61c8466e-a2ab-4ded-b325-595d930c7bc2) |
| ![Block division 4 proc  2000x 20000 100000](https://github.com/user-attachments/assets/045ebc82-a0e5-4bf8-ae27-8dbfec4c8269) |
