#!/usr/bin/env bash

echo -e "\033[0;32mBuild 1. Monte-Carlo\033[0m"
export TARGET=MONTE_CARLO
make -f Makefile

echo -e "\033[0;32mBuild 2. Mandelbrot\033[0m"
export TARGET=MANDELBROT
make -f Makefile

echo -e "\033[0;32mBuild 3. rwlock implementation\033[0m"
export TARGET=RWLOCK
make -f Makefile
export VERSION=MY_RWLOCK
make -f Makefile
