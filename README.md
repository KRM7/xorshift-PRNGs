# Xorshift PRNGs
Some xorshift based PRNG implementations in C++ adapted from https://prng.di.unimi.it/.

All of the PRNGs implemented work with the distributions in the standard library (eg. `std::uniform_real_distribution`), and qualify as `std::uniform_random_bit_generator`.
<br>

The generators implemented are: <br>
* 32-bit:
  * xoshiro128p
  * xoshiro128ss
* 64-bit:
  * xoshiro256p
  * xoshiro256ss
  * xoroshiro128p

### Usage example

```cpp
#include "xorshift.h"

using namespace xorshift;

in main()
{
    /* Initialize one of the engines. */
    xoshiro128p gen{ std::random_device{}() };

    /* Get a distribution. */
    std::uniform_real_distribution<double> dist(1.0, 3.0);

    /* Generate and print some random numbers. */
    for (int i = 0; i < 10; i++)
    {
        std::cout << dist(gen) << "\n";
    }

    return 0;
}
```

### Benchmark results

Using the mersenne twister engines as the baseline.
```
g++ -O3 -std=c++17
```

```
gcc 8.1.0

Pass 0
mersenne:	0.583545 s
mersenne64:	1.05663 s
xoroshiro128p:	0.396048 s
xoshiro256p:	0.382495 s
xoshiro128p:	0.525778 s
xoshiro256ss:	0.468575 s
xoshiro128ss:	0.294529 s

Pass 1
mersenne:	0.547954 s
mersenne64:	1.0341 s
xoroshiro128p:	0.366878 s
xoshiro256p:	0.354722 s
xoshiro128p:	0.518871 s
xoshiro256ss:	0.470093 s
xoshiro128ss:	0.270114 s

Pass 2
mersenne:	0.542493 s
mersenne64:	1.00261 s
xoroshiro128p:	0.373166 s
xoshiro256p:	0.362032 s
xoshiro128p:	0.509503 s
xoshiro256ss:	0.475467 s
xoshiro128ss:	0.264851 s
```