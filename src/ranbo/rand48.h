
// Copyright (c) 1993 Martin Birgmeier
// All rights reserved.
//
// You may redistribute unmodified or modified versions of this source
// code provided that the above copyright notice and this and the
// following conditions are retained.
//
// This software is provided ``as is'', and comes with no warranties
// of any kind. I shall in no event be liable for anything that happens
// to anyone/anything when using this software.

/**
 * The rand48 PRNG generates pseudo-random numbers using a linear congruential
 * algorithm working on integers 48 bits in size.
 * The particular formula employed is:
 *
 * r(n+1) = (a * r(n) + c) mod m
 *
 * where the values are for the multiplicand a = 0xfdeece66d = 25214903917 and
 * the addend c = 0xb = 11. The modulo is always fixed at m = 2 ** 48.
 * r(n) is called the seed of the random number generator.
 */

#pragma once

#include "ranbo.h"

#define RAND48_SEED_0 (0x330e)
#define RAND48_MULT_0 (0xe66d)
#define RAND48_MULT_1 (0xdeec)
#define RAND48_MULT_2 (0x0005)
#define RAND48_ADD (0x000b)

/// \struct ranbo_rand48
/// An opaque structure representing the internal state of the rand48 PRNG.
struct ranbo_rand48
{
    /// The seeds of the PRNG.
    uint16_t seed[3];
};

/// ranbo_rand48_set_seed() - initialize the internal seed buffer r(n)
///
/// The 32 bits of the seed value are copied into the upper 32 bits of r(n),
/// with the lower 16 bits of r(n) arbitrarily being set to 0x330e.
/// Additionally, the constant multiplicand and addend of the algorithm are
/// reset to the default values given above.
///
/// @param state - pointer to the PRNG state
/// @param seed - the seed value to be used
inline void ranbo_rand48_set_seed(struct ranbo_rand48* state, uint32_t seed)
{
    state->seed[0] = RAND48_SEED_0;
    state->seed[1] = (uint16_t)seed;
    state->seed[2] = (uint16_t)(seed >> 16);
}

/// ranbo_rand48_generate() - return values of type uint32_t in the range [0,
/// 2**31-1].
///
/// The high-order (32) bits of r(n+1) are loaded into the lower bits of the
/// returned value.
///
/// @param state - pointer to the PRNG state
/// @return - a pseudo-random value of type uint32_t
static uint32_t ranbo_rand48_generate(struct ranbo_rand48* state)
{
    uint32_t accu;
    uint16_t temp[2];

    accu = (uint32_t)RAND48_MULT_0 * (uint32_t)state->seed[0] +
           (uint32_t)RAND48_ADD;
    temp[0] = (uint16_t)accu; // lower 16 bits
    accu >>= sizeof(uint16_t) * 8;
    accu += (uint32_t)RAND48_MULT_0 * (uint32_t)state->seed[1] +
            (uint32_t)RAND48_MULT_1 * (uint32_t)state->seed[0];
    temp[1] = (uint16_t)accu; // middle 16 bits
    accu >>= sizeof(uint16_t) * 8;
    accu += RAND48_MULT_0 * state->seed[2] + RAND48_MULT_1 * state->seed[1] +
            RAND48_MULT_2 * state->seed[0];
    state->seed[0] = temp[0];
    state->seed[1] = temp[1];
    state->seed[2] = (uint16_t)accu;
    return ((uint32_t)state->seed[2] << 16) | ((uint32_t)state->seed[1]);
}
