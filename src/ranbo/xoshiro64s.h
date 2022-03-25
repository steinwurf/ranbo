
#pragma once

#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/types.h>
#else
#include <stdint.h>
#include <string.h>
#endif

#define MULT_CONSTANT 0x9E3779BB
#define MIX_CONSTANT_1 0x9e3779b97f4a7c15uLL
#define MIX_CONSTANT_2 0xbf58476d1ce4e5b9uLL
#define MIX_CONSTANT_3 0x94d049bb133111ebuLL

struct ranbo_xoshiro64s
{
    uint64_t seed = 1;
    uint32_t seed_parts[2] = {3793791033U, 2065550767U};
};

uint32_t ranbo_detail_xoshiro64s_rotate(uint32_t number, int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (32 - bits_to_rotate));
}

uint64_t ranbo_detail_xoshiro64s_split_and_mix_seed(uint64_t* seed)
{
    uint64_t mix = (*seed += MIX_CONSTANT_1);
    mix = (mix ^ (mix >> 30)) * MIX_CONSTANT_2;
    mix = (mix ^ (mix >> 27)) * MIX_CONSTANT_3;
    return mix ^ (mix >> 31);
}

// Do not pass 0 to this function please.
void ranbo_xoshiro64s_set_seed(struct ranbo_xoshiro64s* generator,
                               uint64_t seed)
{
    generator->seed = seed;
    uint64_t temp_seed = 0;
    temp_seed = ranbo_detail_xoshiro64s_split_and_mix_seed(&temp_seed);
    generator->seed_parts[0] =
        (uint32_t)((temp_seed & 0xFFFFFFFF00000000LL) >> 32);
    generator->seed_parts[1] = (uint32_t)(temp_seed & 0xFFFFFFFFLL);
}

uint32_t ranbo_xoshiro64s_generate(struct ranbo_xoshiro64s* generator)
{
    uint32_t s1 = generator->seed_parts[1];
    uint32_t result = generator->seed_parts[0] * MULT_CONSTANT;

    s1 ^= generator->seed_parts[0];
    generator->seed_parts[0] =
        ranbo_detail_xoshiro64s_rotate(generator->seed_parts[0], 26) ^
        generator->seed_parts[1] ^ (generator->seed_parts[1] << 9);
    generator->seed_parts[1] =
        ranbo_detail_xoshiro64s_rotate(generator->seed_parts[1], 13);
    generator->seed =
        ((uint64_t)generator->seed_parts[0]) << 32 | generator->seed_parts[1];
    return result;
}
