This first version of the program was made for Algorithms and complexity course. Expanding it, we want to create parallel version and compare time efficiency

## Patterns
Here was used 
* **Strategy pattern** for choosing parallel and sequential strategy of computation
* **Factory** for choosing the mode of the running the program

Here is the data from benchmark

### Graph Size: 25 vertices

Density 0.1: Sequential=2.3ms, Parallel=2.5ms, Speedup=0.91x

Density 0.3: Sequential=0.6ms, Parallel=0.5ms, Speedup=1.34x

Density 0.5: Sequential=0.7ms, Parallel=0.6ms, Speedup=1.16x

Density 0.7: Sequential=0.8ms, Parallel=0.7ms, Speedup=1.13x

### Graph Size: 50 vertices


Density 0.1: Sequential=0.9ms, Parallel=3.9ms, Speedup=0.23x

Density 0.3: Sequential=1.7ms, Parallel=1.5ms, Speedup=1.15x

Density 0.5: Sequential=2.3ms, Parallel=2.8ms, Speedup=0.84x

Density 0.7: Sequential=3.1ms, Parallel=3.2ms, Speedup=0.98x

### Graph Size: 75 vertices


Density 0.1: Sequential=2.3ms, Parallel=7.4ms, Speedup=0.31x

Density 0.3: Sequential=4.0ms, Parallel=3.9ms, Speedup=1.02x

Density 0.5: Sequential=7.0ms, Parallel=6.9ms, Speedup=1.01x

Density 0.7: Sequential=9.1ms, Parallel=8.5ms, Speedup=1.07x

### Graph Size: 100 vertices


Density 0.1: Sequential=3.8ms, Parallel=3.8ms, Speedup=1.01x

Density 0.3: Sequential=8.8ms, Parallel=8.4ms, Speedup=1.05x

Density 0.5: Sequential=14.3ms, Parallel=14.2ms, Speedup=1.01x

Density 0.7: Sequential=19.9ms, Parallel=19.2ms, Speedup=1.04x

### Graph Size: 150 vertices

Density 0.1: Sequential=10.3ms, Parallel=13.1ms, Speedup=0.79x

Density 0.3: Sequential=28.4ms, Parallel=29.3ms, Speedup=0.97x

Density 0.5: Sequential=42.4ms, Parallel=43.0ms, Speedup=0.99x

Density 0.7: Sequential=72.4ms, Parallel=71.8ms, Speedup=1.01x

### Graph Size: 200 vertices

Density 0.1: Sequential=20.9ms, Parallel=22.0ms, Speedup=0.95x

Density 0.3: Sequential=81.1ms, Parallel=60.1ms, Speedup=1.35x

Density 0.5: Sequential=110.2ms, Parallel=97.8ms, Speedup=1.13x

Density 0.7: Sequential=166.4ms, Parallel=165.7ms, Speedup=1.00x

---------------------------------------------

So we can make a conclusion that parallel computing is working bad with too much small density. In most cases the best results was for 0.3 density. But the more vertices we have the smaller difference for densities become