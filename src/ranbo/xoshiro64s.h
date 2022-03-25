
#pragma once

#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/types.h>
#else
#include <stdint.h>
#include <string.h>
#endif

#define MULT_CONSTANT 0x9E3779BB

struct ranbo_xoshiro64s
{
    uint64_t seed = 2203318231040;
    uint32_t seed_parts[2] = {513U, 8192U};
};

uint32_t ranbo_detail_xoshiro64s_rotate(uint32_t number, int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (32 - bits_to_rotate));
}

// Do not pass 0 to this function please.
void ranbo_xoshiro64s_set_seed(struct ranbo_xoshiro64s* generator,
                               uint64_t seed)
{
    generator->seed = seed;
    generator->seed_parts[0] = (uint32_t)((seed & 0xFFFFFFFF00000000LL) >> 32);
    generator->seed_parts[1] = (uint32_t)(seed & 0xFFFFFFFFLL);
}

uint32_t ranbo_xoshiro64s_generate(struct ranbo_xoshiro64s* generator)
{
    const uint32_t s0 = generator->seed_parts[0];
    uint32_t s1 = generator->seed_parts[1];
    uint32_t result = s0 * MULT_CONSTANT;

    s1 ^= s0;
    generator->seed_parts[0] =
        ranbo_detail_xoshiro64s_rotate(s0, 26) ^ s1 ^ (s1 << 9);
    generator->seed_parts[1] = ranbo_detail_xoshiro64s_rotate(s1, 13);
    generator->seed =
        ((uint64_t)generator->seed_parts[0]) << 32 | generator->seed_parts[1];
    return result;
}
