// Copyright (c) Steinwurf ApS 2020.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// https://www.steinwurf.com/licensing

#pragma once

#include "ranbo.h"

#define XOSHIRO256SS_MIX_CONSTANT_1 0x9e3779b97f4a7c15uLL
#define XOSHIRO256SS_MIX_CONSTANT_2 0xbf58476d1ce4e5b9uLL
#define XOSHIRO256SS_MIX_CONSTANT_3 0x94d049bb133111ebuLL

/// \struct ranbo_xoshiro256ss
/// An opaque structure representing the internal state of the Xoshiro256**
/// PRNG.
struct ranbo_xoshiro256ss
{
    /// The seeds of the RNG.
    uint64_t seed[4];
};

inline uint64_t ranbo_detail_xoshiro256ss_split_and_mix_seed(uint64_t* seed)
{
    uint64_t mix = (*seed += XOSHIRO256SS_MIX_CONSTANT_1);
    mix = (mix ^ (mix >> 30)) * XOSHIRO256SS_MIX_CONSTANT_2;
    mix = (mix ^ (mix >> 27)) * XOSHIRO256SS_MIX_CONSTANT_3;
    return mix ^ (mix >> 31);
}

inline uint64_t ranbo_detail_xoshiro256ss_rotate(uint64_t number,
                                                 int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (64 - bits_to_rotate));
}

/// Sets the internal state of the PRNG.
///
/// The seed is used to generate 4 64-bit
/// seed components by using a splitmix64 algorithm. See e.g
/// https://github.com/svaarala/duktape/blob/master/misc/splitmix64.c, for
/// more information on splitmix64. These four seed components
/// are used for generating the random numbers.
///
/// @param state The state of the PRNG
/// @param seed The seed to use for the PRNG
inline void ranbo_xoshiro256ss_set_seed(struct ranbo_xoshiro256ss* state,
                                        uint64_t seed)
{
    state->seed[0] = ranbo_detail_xoshiro256ss_split_and_mix_seed(&seed);
    state->seed[1] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(state->seed[0]));
    state->seed[2] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(state->seed[1]));
    state->seed[3] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(state->seed[2]));
}

/// Generates a random number from the PRNG state.
///
/// Based on the implementation from David Blackman and Sebastiano Vigna (2018).
/// See https://prng.di.unimi.it/xoshiro256starstar.c for more information.
///
/// @param state The state of the PRNG
/// @return A random number as a uint64_t
inline uint64_t ranbo_xoshiro256ss_generate(struct ranbo_xoshiro256ss* state)
{
    const uint64_t result =
        ranbo_detail_xoshiro256ss_rotate(state->seed[1] * 5, 7) * 9;
    const uint64_t temp = state->seed[1] << 17;
    state->seed[2] ^= state->seed[0];
    state->seed[3] ^= state->seed[1];
    state->seed[1] ^= state->seed[2];
    state->seed[0] ^= state->seed[3];
    state->seed[2] ^= temp;
    state->seed[3] = ranbo_detail_xoshiro256ss_rotate(state->seed[3], 45);
    return result;
}
