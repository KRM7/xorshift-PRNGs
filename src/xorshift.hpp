/* Copyright (c) 2022 Kriszti?n Rug?si. Subject to the MIT license. */

/**
 * Implementations of some xorshift based PRNGs from https://prng.di.unimi.it/.
 * 
 * All of the PRNGs implemented in this work with the distributions in the standard library (eg. std::uniform_real_distribution),
 * and qualify as std::uniform_random_bit_generator.
 */

#ifndef XORSHIFT_H
#define XORSHIFT_H

#include <limits>
#include <cstdint>
#include <array>
#include <bit>

namespace xorshift
{
    /**
     * Splitmix64 PRNG adapted from https://prng.di.unimi.it/splitmix64.c
     * Generates 64-bit numbers, used for the initialization of the other PRNGs.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class splitmix64 final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr splitmix64(uint64_t seed) noexcept
            : state(seed)
        {}

        constexpr result_type operator()() noexcept
        {
            state += 0x9e3779b97f4a7c15;
            result_type z = state;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
            z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
            return z ^ (z >> 31);
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            state = seed;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        state_type state;
    };

    /**
     * xoroshiro128+ PRNG adapted from https://prng.di.unimi.it/xoroshiro128plus.c
     * Generates 64-bit numbers. 16 byte state.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoroshiro128p final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoroshiro128p(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen() };
        }

        explicit constexpr xoroshiro128p(const std::array<state_type, 2>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            const state_type s0 = state[0];
            state_type s1 = state[1];
            const result_type result = s0 + s1;

            s1 ^= s0;
            state[0] = std::rotl<state_type>(s0, 24U) ^ s1 ^ (s1 << 16);
            state[1] = std::rotl<state_type>(s1, 37U);

            return result;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen() };
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 2> state;
    };

    /**
     * xoshiro256+ PRNG adapted from https://prng.di.unimi.it/xoshiro256plus.c
     * Generates 64-bit numbers. 32 byte state.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro256p final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoshiro256p(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen(), seed_seq_gen(), seed_seq_gen() };
        }

        explicit constexpr xoshiro256p(const std::array<state_type, 4>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = state[0] + state[3];
            const state_type t = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = std::rotl<state_type>(state[3], 45U);

            return result;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen(), seed_seq_gen(), seed_seq_gen() };
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;
    };

    /**
     * xoshiro128+ PRNG adapted from https://prng.di.unimi.it/xoshiro128plus.c
     * Generates 32-bit numbers. 16 byte state.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro128p final
    {
    public:
        using result_type = uint32_t;
        using state_type = uint32_t;

        explicit constexpr xoshiro128p(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()) };
        }

        explicit constexpr xoshiro128p(const std::array<state_type, 4>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = state[0] + state[3];
            const state_type t = state[1] << 9;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = std::rotl<state_type>(state[3], 11U);

            return result;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()) };
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;
    };

    /**
     * xoshiro256** PRNG adapted from https://prng.di.unimi.it/xoshiro256starstar.c
     * Generates 64-bit numbers. 32 byte state. Highest quality 64 bit generator.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro256ss final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoshiro256ss(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen(), seed_seq_gen(), seed_seq_gen() };
        }

        explicit constexpr xoshiro256ss(const std::array<state_type, 4>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = std::rotl<state_type>(state[1] * 5U, 7U) * 9U;
            const state_type t = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = std::rotl<state_type>(state[3], 45U);

            return result;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { seed_seq_gen(), seed_seq_gen(), seed_seq_gen(), seed_seq_gen() };
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;
    };

    /**
     * xoshiro128** PRNG adapted from https://prng.di.unimi.it/xoshiro128starstar.c
     * Generates 32-bit numbers. 16 byte state. Highest quality 32 bit generator.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro128ss final
    {
    public:
        using result_type = uint32_t;
        using state_type = uint32_t;
        
        explicit constexpr xoshiro128ss(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()) };
        }

        explicit constexpr xoshiro128ss(const std::array<state_type, 4>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = std::rotl<state_type>(state[1] * 5U, 7U) * 9U;
            const state_type t = state[1] << 9;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = std::rotl<state_type>(state[3], 11U);

            return result;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            splitmix64 seed_seq_gen(seed);
            state = { state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()), state_type(seed_seq_gen()) };
        }
        
        static constexpr result_type min() noexcept    
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;
    };

    class sfc64 final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr sfc64(uint64_t seed) noexcept
            : state({ seed, seed, seed, 1 })
        {
            warmup();
        }

        explicit constexpr sfc64(std::array<state_type, 4>& state) noexcept
            : state(state)
        {}

        constexpr result_type operator()() noexcept
        {
            result_type ret = state[0] + state[1] + state[3];
            state[3]++;

            state[0] = state[1] ^ (state[1] >> 11);
            state[1] = state[2] + (state[2] << 3);
            state[2] = std::rotl<state_type>(state[2], 24) + ret;

            return ret;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            state = { seed, seed, seed, 1 };
            warmup();
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;

        constexpr void warmup() noexcept
        {
            for (size_t i = 0; i < 12; i++) 
            {
                operator()();
            }
        }
    };

    class xorshift64s final
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xorshift64s(uint64_t seed) noexcept
            : state(seed)
        {}

        constexpr result_type operator()() noexcept
        {
            state ^= state >> 12;
            state ^= state << 25;
            state ^= state >> 27;
            return state * 0x2545F4914F6CDD1DULL;
        }

        constexpr void discard(size_t n) noexcept
        {
            while (n--) operator()();
        }

        constexpr void seed(uint64_t seed) noexcept
        {
            state = seed;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::min();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        state_type state;
    };

} // namespace xorshift

#endif