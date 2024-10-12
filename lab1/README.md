# Distributed computing

---

## Lab 1
### How to build
##### All tasks:
```sh
chmod +x ./build.sh
./build.sh
cd bin
```
##### Each task is separated:
```sh
TARGET=MONTE_CARLO make
TARGET=MANDELBROT make
TARGET=RWLOCK make
cd bin
```

### How to run
```sh
./monte-carlo.o # run 1. monte-carlo 
./mandelbrot.o  # run 2. mandelbrot
./rwlock.o      # run 3. rwlock implementation
```
