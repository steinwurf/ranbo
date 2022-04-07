
// Copyright (c) Steinwurf ApS 2020.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// https://www.steinwurf.com/licensing

#pragma once

#include "ranbo.h"

#define MULT_CONSTANT 0x9E3779BB
#define MIX_CONSTANT_1 0x9e3779b97f4a7c15uLL
#define MIX_CONSTANT_2 0xbf58476d1ce4e5b9uLL
#define MIX_CONSTANT_3 0x94d049bb133111ebuLL

/// \struct ranbo_xoshiro64s
/// An opaque structure representing the internal state of the Xoshiro64* PRNG.
struct ranbo_xoshiro64s
{
    /// The seeds of the RNG.
    uint32_t seed[2];
};

inline uint32_t ranbo_detail_xoshiro64s_rotate(uint32_t number,
                                               int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (32 - bits_to_rotate));
}

inline uint64_t ranbo_detail_xoshiro64s_split_and_mix_seed(uint64_t* seed)
{
    uint64_t mix = (*seed += MIX_CONSTANT_1);
    mix = (mix ^ (mix >> 30)) * MIX_CONSTANT_2;
    mix = (mix ^ (mix >> 27)) * MIX_CONSTANT_3;
    return mix ^ (mix >> 31);
}

/// Sets the seed in the internal state.
///
/// The seed is mixed using a splitmix64
/// algorithm and then split into two 32 bit components. See e.g
/// https://github.com/svaarala/duktape/blob/master/misc/splitmix64.c, for
/// more information.
///
/// @param state The internal state of the PRNG.
/// @param seed The seed to set.
inline void ranbo_xoshiro64s_set_seed(struct ranbo_xoshiro64s* state,
                                      uint64_t seed)
{
    uint64_t temp_seed = seed;
    temp_seed = ranbo_detail_xoshiro64s_split_and_mix_seed(&temp_seed);
    state->seed[0] = (uint32_t)((temp_seed & 0xFFFFFFFF00000000LL) >> 32);
    state->seed[1] = (uint32_t)(temp_seed & 0xFFFFFFFFLL);
}

/// Generates a random number from the internal state.
///
/// Based on the implementation from David Blackman and Sebastiano Vigna (2016).
/// See https://prng.di.unimi.it/xoroshiro64star.c for more information.
///
/// @param state The internal state of the PRNG.
/// @return The generated random number as a uint32_t.
inline uint32_t ranbo_xoshiro64s_generate(struct ranbo_xoshiro64s* state)
{
    uint32_t s1 = state->seed[1];
    uint32_t result = state->seed[0] * MULT_CONSTANT;

    s1 ^= state->seed[0];
    state->seed[0] = ranbo_detail_xoshiro64s_rotate(state->seed[0], 26) ^
                     state->seed[1] ^ (state->seed[1] << 9);
    state->seed[1] = ranbo_detail_xoshiro64s_rotate(state->seed[1], 13);
    return result;
}
