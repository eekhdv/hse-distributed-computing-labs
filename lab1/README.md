# Distributed computing

---

## lab 1

### pi calculation (Monte-Carlo method)

`./src/monte-carlo.c`

> We get the number of points as input. The Monte Carlo algorithm is that we generate random points (x, y) and check that x^2+ y^2 <=1. If yes, then increase the point counter in the circle. At the end, you need to divide the number of points in the circle by the total number of points.

##### How did I parallelize the algorithm:
I  create an array of points,  which I fill with random  points  (x,  y). I give each  thread  a  pointer  to a part of this  array  and a sub-size  equal to the number of points  to  process.  After  that,  each  thread  starts  working  with the points,  performing the check  operation `x^2 + y^2 <= 1` and, depending on this, increasing the counter for the number of points in the circle. In the end, I take the resulting number from each thread and calculate the number pi.

##### Test results
<details>
  <summary>Screenshots</summary>

  ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/monte-carlo.png)
</details>

### Mandelbrot set 

`./src/mandelbrot.c`

> The Mandelbrot set is the set of all numbers C for which the sequence `z(n+1) = z(n)^2 + c` is bounded for all values of n. The problem boils down to finding the number c, which is a complex number.

##### How did I parallelize the task:
I have created global variables for all threads, one contains the number of points found, and the other variable is an array with points. As soon as the required number of points is typed, all threads stop working. At the beginning, depending on the number of threads, I calculate x_start and x_end - these are variables that limit the scope of the search for Mandelbrot points for each thread. They are calculated in such a way that none of the threads iterates over the numbers that another thread could iterate over. Also, after finding the number, the thread locks mutex and changes the global variable with the number of points and the array of points.

##### Test results
<details>
  <summary>Screenshots</summary>

  ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/mandelbrot.png)
</details>

### read-write lock implementation (pthread)

`./src/pth_ll_rwl/rwlock.c`

To create a structure for read-write lock, the following fields are given in the task:
* Mutex
* Two conditional variables (one for readers, the other for writers)
* Reader counter (how many threads are currently reading)
* Counter for the number of threads waiting to receive a read lock
* Count of the number of threads waiting to receive a write lock
* A flag indicating whether at least one writer has been blocked at the moment

##### The logic of my read-write lock:

`wrlock()`:

Increasing the number of waiting writers. Next, if the rwlock is currently blocked by the reader, then I wait until it releases, otherwise, I wait until all readers stop reading. Next, I reduce the number of waiting writers and put a flag that there is currently one writer.

`rdlock()`:

Increasing the counter for the number of waiting readers. I'm waiting for the writer to release the conditional variable. Next, I reduce the number of waiting readers and increase the reader counter.

`unlock()`:

If rwlock is a writer, then I remove the writer's blocking flag. If the number of waiting writers is greater than 0, then I notify them. If there are no waiting writers, then I notify all waiting readers. If the rwlock was blocked by the reader, then if the reader is not the last, then I simply decrease the reader counter, and if it is the last reader, then I inform the writers that the stream is released.

##### Test results
<details>
  <summary>Screenshots</summary>

  
   | My rwlock          |  rwlock |
   :-------------------------:|:-------------------------:
   1 thread ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/my_rwlock_1.png) | 1 thread ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/rwlock_1.png)
   8 threads ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/my_rwlock_2.png) | 8 threads ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/rwlock_2.png)
   4 threads ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/my_rwlock_3.png) | 4 threads ![](https://github.com/eekhdv/hse-distributed-computing-labs/blob/lab1/results/lab1/results/rwlock_3.png)
</details>


### how to build
##### all tasks:
```sh
chmod +x ./build.sh
./build.sh
cd bin
```
##### each task is separated:
```sh
TARGET=MONTE_CARLO make
TARGET=MANDELBROT make
TARGET=RWLOCK make
cd bin
```

### how to run
```sh
cd bin
./monte-carlo.o # run 1. monte-carlo 
./mandelbrot.o  # run 2. mandelbrot
./rwlock.o      # run 3. rwlock implementation
```
