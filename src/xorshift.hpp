/* Copyright (c) 2022 Krisztián Rugási. Subject to the MIT license. */

/**
 * Implementations of some xorshift based PRNGs from https://prng.di.unimi.it/.
 * 
 * All of the PRNGs implemented in this work with the distributions in the standard library (eg. std::uniform_real_distribution),
 * and qualify as std::uniform_random_bit_generator.
 * All of them are faster than the mersenne twister engines implemented in the standard library.
 */

#ifndef XORSHIFT_H
#define XORSHIFT_H

#include <limits>
#include <cstdint>
#include <array>

namespace xorshift
{
    /**
     * Splitmix64 PRNG adapted from https://prng.di.unimi.it/splitmix64.c
     * Generates 64-bit numbers, only used for the initialization of the other PRNGs.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class splitmix64
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr splitmix64(state_type seed) : state(seed) {}

        constexpr result_type operator()() noexcept
        {
            state += 0x9e3779b97f4a7c15;
            result_type z = state;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
            z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
            return z ^ (z >> 31);
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
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
    class xoroshiro128p
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoroshiro128p(uint64_t seed)
        {
            splitmix64 seed_seq_gen(seed);
            state[0] = seed_seq_gen();
            state[1] = seed_seq_gen();
        }

        explicit constexpr xoroshiro128p(const std::array<state_type, 2>& state) : state(state) {}

        constexpr result_type operator()() noexcept
        {
            const state_type s0 = state[0];
            state_type s1 = state[1];
            const result_type result = s0 + s1;

            s1 ^= s0;
            state[0] = rotl64(s0, 24U) ^ s1 ^ (s1 << 16);
            state[1] = rotl64(s1, 37U);

            return result;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 2> state;

        static constexpr state_type rotl64(state_type x, unsigned k) noexcept
        {
            return (x << k) | (x >> (64U - k));
        }
    };

    /**
     * xoshiro256+ PRNG adapted from https://prng.di.unimi.it/xoshiro256plus.c
     * Generates 64-bit numbers. 32 byte state.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro256p
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoshiro256p(uint64_t seed)
        {
            splitmix64 seed_seq_gen(seed);
            state[0] = seed_seq_gen();
            state[1] = seed_seq_gen();
            state[2] = seed_seq_gen();
            state[3] = seed_seq_gen();
        }

        explicit constexpr xoshiro256p(const std::array<state_type, 4>& state) : state(state) {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = state[0] + state[3];
            const state_type t = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = rotl64(state[3], 45U);

            return result;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;

        static constexpr state_type rotl64(state_type x, unsigned k) noexcept
        {
            return (x << k) | (x >> (64U - k));
        }
    };

    /**
     * xoshiro128+ PRNG adapted from https://prng.di.unimi.it/xoshiro128plus.c
     * Generates 32-bit numbers. 16 byte state.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro128p
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoshiro128p(uint64_t seed)
        {
            splitmix64 seed_seq_gen(seed);
            state[0] = state_type(seed_seq_gen());
            state[1] = state_type(seed_seq_gen());
            state[2] = state_type(seed_seq_gen());
            state[3] = state_type(seed_seq_gen());
        }

        explicit constexpr xoshiro128p(const std::array<state_type, 4>& state) : state(state) {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = state[0] + state[3];
            const state_type t = state[1] << 9;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = rotl32(state[3], 11U);

            return result;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;

        static constexpr state_type rotl32(state_type x, unsigned k) noexcept
        {
            return (x << k) | (x >> (32U - k));
        }
    };

    /**
     * xoshiro256** PRNG adapted from https://prng.di.unimi.it/xoshiro256starstar.c
     * Generates 64-bit numbers. 32 byte state. Highest quality 64 bit generator.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro256ss
    {
    public:
        using result_type = uint64_t;
        using state_type = uint64_t;

        explicit constexpr xoshiro256ss(uint64_t seed)
        {
            splitmix64 seed_seq_gen(seed);
            state[0] = seed_seq_gen();
            state[1] = seed_seq_gen();
            state[2] = seed_seq_gen();
            state[3] = seed_seq_gen();
        }

        explicit constexpr xoshiro256ss(const std::array<state_type, 4>& state) : state(state) {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = rotl64(state[1] * 5, 7U) * 9;
            const state_type t = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = rotl64(state[3], 45U);

            return result;
        }

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;

        static constexpr state_type rotl64(state_type x, unsigned k) noexcept
        {
            return (x << k) | (x >> (64U - k));
        }
    };

    /**
     * xoshiro128** PRNG adapted from 
     * Generates 32-bit numbers. 16 byte state. Highest quality 32 bit generator.
     * Works with the standard library distributions, qualifies as std::uniform_random_bit_generator
     */
    class xoshiro128ss
    {
    public:
        using result_type = uint32_t;
        using state_type = uint32_t;
        
        explicit constexpr xoshiro128ss(uint64_t seed)
        {
            splitmix64 seed_seq_gen(seed);
            state[0] = state_type(seed_seq_gen());
            state[1] = state_type(seed_seq_gen());
            state[2] = state_type(seed_seq_gen());
            state[3] = state_type(seed_seq_gen());
        }

        explicit constexpr xoshiro128ss(const std::array<state_type, 4>& state) : state(state) {}

        constexpr result_type operator()() noexcept
        {
            const result_type result = rotl32(state[1] * 5, 7U) * 9;
            const state_type t = state[1] << 9;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = rotl32(state[3], 11U);

            return result;
        }
        
        static constexpr result_type min() noexcept    
        {
            return std::numeric_limits<result_type>::lowest();
        }
        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        std::array<state_type, 4> state;

        static constexpr state_type rotl32(state_type x, unsigned k) noexcept
        {
            return (x << k) | (x >> (32U - k));
        }
    };

} // namespace xorshift

#endif