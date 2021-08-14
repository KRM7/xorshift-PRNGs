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

Using the mersenne twister engines as the baselines for comparisons.
```
g++ -std=c++17 -DNDEBUG -O2 -flto
```

```
gcc 10.2.0

Pass 0
mersenne:	  1.23089 s
mersenne64:	0.964539 s
xoroshiro128p:	0.346801 s
xoshiro256p:	0.346446 s
xoshiro128p:	0.369133 s
xoshiro256ss:	0.361454 s
xoshiro128ss:	0.47015 s

Pass 1
mersenne:	  1.22467 s
mersenne64:	0.959018 s
xoroshiro128p:	0.347749 s
xoshiro256p:	0.352565 s
xoshiro128p:	0.418605 s
xoshiro256ss:	0.360082 s
xoshiro128ss:	0.469966 s

Pass 2
mersenne:	  1.25978 s
mersenne64:	0.959827 s
xoroshiro128p:	0.364393 s
xoshiro256p:	0.360509 s
xoshiro128p:	0.369169 s
xoshiro256ss:	0.361206 s
xoshiro128ss:	0.468972 s
```
