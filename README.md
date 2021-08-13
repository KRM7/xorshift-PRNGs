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
g++ -std=c++20 -O2 -flto -DNDEBUG
```

```
gcc 10.2.0

Pass 0
mersenne:	    1.26325 s
mersenne64:	    0.993472 s
xoroshiro128p:	0.438569 s
xoshiro256p:	0.402429 s
xoshiro128p:	0.399804 s
xoshiro256ss:	0.444019 s
xoshiro128ss:	0.498351 s

Pass 1
mersenne:	    1.14937 s
mersenne64: 	0.901422 s
xoroshiro128p:	0.346773 s
xoshiro256p:	0.347103 s
xoshiro128p:	0.367864 s
xoshiro256ss:	0.360885 s
xoshiro128ss:	0.498022 s

Pass 2
mersenne:	    1.13552 s
mersenne64:	    0.900177 s
xoroshiro128p:	0.34583 s
xoshiro256p:	0.347364 s
xoshiro128p:	0.369959 s
xoshiro256ss:	0.362179 s
xoshiro128ss:	0.497302 s
```
