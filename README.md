# Cache Perfomance Model

This tool provides a statistics on cache perfomance for L1 and L2 Cache configurations.


## Getting Started

### Dependencies

*Python3 


### Executing program

* How to run the program
* Step-by-step bullets
```
make all 
python3 run_script.py
```
 
### Configurations
By default the script runs the different configurations of cache blocks, cache size, cache associativity for both L1 and L2 caches and returns the result of the cache hits, misses and miss rate. 

To run individual workload : 
```
./run <block_size> <L1_cache_size> <L1_cache_assoc> <filename> <L2_cache_size> <L2_cache_assoc>
```
Note : L2 configurations are optional.

