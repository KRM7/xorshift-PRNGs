# Xorshift PRNGs
Some xorshift based PRNG implementations in C++ adapted from https://prng.di.unimi.it/.

All of the PRNGs implemented work with the distributions in the standard library, and qualify as `std::uniform_random_bit_generator`.  

The generators implemented are:  
* 32-bit:
  * xoshiro128p
  * xoshiro128ss
* 64-bit:
  * xoroshiro128p
  * xoshiro256p
  * xoshiro256ss

## Usage example

```cpp
#include "xorshift.hpp"

using namespace xorshift;

in main()
{
    /* Initialize one of the engines. */
    xoshiro128p gen{ std::random_device{}() };

    /* Get a distribution. */
    std::uniform_real_distribution<double> dist(1.0, 3.0);

    /* Print a random number. */
    std::cout << dist(gen) << "\n";
}
```

## Benchmark results

Using the mersenne twister and ranlux engines of the std library for comparison.
```
g++ -std=c++20 -DNDEBUG -O2 -flto -fno-unroll-loops -fno-move-loop-invariants
```

```
gcc 11.2.0

-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
DBL<mt19937>             22.4 ns         22.5 ns     32000000
DBL<mt19937_64>          19.0 ns         18.8 ns     37333333
DBL<ranlux24_base>       27.7 ns         27.6 ns     24888889
DBL<ranlux48_base>       17.4 ns         17.6 ns     40727273

DBL<xoroshiro128p>       6.60 ns         6.45 ns     89600000
DBL<xoshiro256p>         6.55 ns         6.56 ns    112000000
DBL<xoshiro128p>         6.93 ns         6.80 ns     89600000
DBL<xoshiro256ss>        6.85 ns         6.98 ns    112000000
DBL<xoshiro128ss>        8.23 ns         8.16 ns     74666667

-------------------------------------------------------------

INT<mt19937>             14.0 ns         14.1 ns     49777778
INT<mt19937_64>          10.8 ns         10.7 ns     64000000
INT<ranlux24_base>       32.7 ns         33.0 ns     21333333
INT<ranlux48_base>       32.2 ns         32.2 ns     21333333

INT<xoroshiro128p>       1.95 ns         1.95 ns    344615385
INT<xoshiro256p>         1.97 ns         1.97 ns    373333333
INT<xoshiro128p>         2.66 ns         2.49 ns    263529412
INT<xoshiro256ss>        1.98 ns         1.95 ns    344615385
INT<xoshiro128ss>        2.63 ns         2.67 ns    263529412
```