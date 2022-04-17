#include <random>
#include <benchmark/benchmark.h>
#include "src/xorshift.hpp"

using namespace std;
using namespace xorshift;

template<typename PRNG>
static void DBL(benchmark::State& state)
{
  PRNG gen{ random_device{}() };
  uniform_real_distribution<double> dist(0.0, 1.0);

  for (auto _ : state)
  {
      benchmark::DoNotOptimize(dist(gen));
  }
}

template<typename PRNG>
static void INT(benchmark::State& state)
{
  PRNG gen{ random_device{}() };
  uniform_int_distribution<int> dist(0, 100);

  for (auto _ : state)
  {
      benchmark::DoNotOptimize(dist(gen));
  }
}

BENCHMARK_TEMPLATE(DBL, mt19937);
BENCHMARK_TEMPLATE(DBL, mt19937_64);
BENCHMARK_TEMPLATE(DBL, ranlux24_base);
BENCHMARK_TEMPLATE(DBL, ranlux48_base);
BENCHMARK_TEMPLATE(DBL, splitmix64);
BENCHMARK_TEMPLATE(DBL, sfc64);
BENCHMARK_TEMPLATE(DBL, xoroshiro128p);
BENCHMARK_TEMPLATE(DBL, xoshiro256p);
BENCHMARK_TEMPLATE(DBL, xoshiro128p);
BENCHMARK_TEMPLATE(DBL, xoshiro256ss);
BENCHMARK_TEMPLATE(DBL, xoshiro128ss);

BENCHMARK_TEMPLATE(INT, mt19937);
BENCHMARK_TEMPLATE(INT, mt19937_64);
BENCHMARK_TEMPLATE(INT, ranlux24_base);
BENCHMARK_TEMPLATE(INT, ranlux48_base);
BENCHMARK_TEMPLATE(INT, splitmix64);
BENCHMARK_TEMPLATE(INT, sfc64);
BENCHMARK_TEMPLATE(INT, xoroshiro128p);
BENCHMARK_TEMPLATE(INT, xoshiro256p);
BENCHMARK_TEMPLATE(INT, xoshiro128p);
BENCHMARK_TEMPLATE(INT, xoshiro256ss);
BENCHMARK_TEMPLATE(INT, xoshiro128ss);

BENCHMARK_MAIN();