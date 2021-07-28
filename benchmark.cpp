#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>

#include "src/xorshift.h"

using namespace std;
using namespace xorshift;

template<typename PRNG>
double benchmarkPRNG(size_t num_rands = 50'000'000)
{
	PRNG gen{ random_device{}() };
	uniform_real_distribution<double> dist(0.0, 100.0);
	vector<double> numbers(num_rands);

	double running_mean_time = 0.0;
	/* Run 5 times and take the average time. */
	for (size_t i = 1; i <= 5; i++)
	{
		auto tbegin = chrono::high_resolution_clock::now();
		generate(numbers.begin(), numbers.end(),
		[&gen, &dist]()
		{
			return dist(gen);
		});
		auto tend = chrono::high_resolution_clock::now();

		auto duration = chrono::duration_cast<chrono::microseconds>(tend - tbegin).count();
		double t = double(duration) / 1E+6;
		running_mean_time += (t - running_mean_time) / i;
	}

	return running_mean_time;
}

int main()
{
	ofstream file;
	file.open("results2.txt");
	size_t num_passes = 3;
	for (size_t i = 0; i < num_passes; i++)
	{
		file << "Pass " << i << "\n";
		file << "mersenne:\t" << benchmarkPRNG<mt19937>() << " s\n";
		file << "mersenne64:\t" << benchmarkPRNG<mt19937_64>() << " s\n";
		file << "xoroshiro128p:\t" << benchmarkPRNG<xoroshiro128p>() << " s\n";
		file << "xoshiro256p:\t" << benchmarkPRNG<xoshiro256p>() << " s\n";
		file << "xoshiro128p:\t" << benchmarkPRNG<xoshiro128p>() << " s\n";
		file << "xoshiro256ss:\t" << benchmarkPRNG<xoshiro256ss>() << " s\n";
		file << "xoshiro128ss:\t" << benchmarkPRNG<xoshiro128ss>() << " s\n\n";
	}
	file.close();

	return 0;
}
