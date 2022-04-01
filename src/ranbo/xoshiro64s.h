
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
    uint32_t seed[2] = {3793791033U, 2065550767U};
};

static uint32_t ranbo_detail_xoshiro64s_rotate(uint32_t number,
                                               int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (32 - bits_to_rotate));
}

static uint64_t ranbo_detail_xoshiro64s_split_and_mix_seed(uint64_t* seed)
{
    uint64_t mix = (*seed += MIX_CONSTANT_1);
    mix = (mix ^ (mix >> 30)) * MIX_CONSTANT_2;
    mix = (mix ^ (mix >> 27)) * MIX_CONSTANT_3;
    return mix ^ (mix >> 31);
}

// Do not pass 0 to this function please.
static void ranbo_xoshiro64s_set_seed(struct ranbo_xoshiro64s* generator,
                                      uint64_t seed)
{
    uint64_t temp_seed = seed;
    temp_seed = ranbo_detail_xoshiro64s_split_and_mix_seed(&temp_seed);
    generator->seed[0] = (uint32_t)((temp_seed & 0xFFFFFFFF00000000LL) >> 32);
    generator->seed[1] = (uint32_t)(temp_seed & 0xFFFFFFFFLL);
}

static uint32_t ranbo_xoshiro64s_generate(struct ranbo_xoshiro64s* generator)
{
    uint32_t s1 = generator->seed[1];
    uint32_t result = generator->seed[0] * MULT_CONSTANT;

    s1 ^= generator->seed[0];
    generator->seed[0] =
        ranbo_detail_xoshiro64s_rotate(generator->seed[0], 26) ^
        generator->seed[1] ^ (generator->seed[1] << 9);
    generator->seed[1] = ranbo_detail_xoshiro64s_rotate(generator->seed[1], 13);
    return result;
}
