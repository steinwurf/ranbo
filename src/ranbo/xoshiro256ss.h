
#pragma once

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define MIX_CONSTANT_1 0x9e3779b97f4a7c15uLL
#define MIX_CONSTANT_2 0xbf58476d1ce4e5b9uLL
#define MIX_CONSTANT_3 0x94d049bb133111ebuLL

struct ranbo_xoshiro256ss
{
    uint64_t seed[4] = {9248179162272254404U, 4989520995034593924U,
                        13959451808893450918U, 2391539541053276776U};
};

static uint64_t ranbo_detail_xoshiro256ss_split_and_mix_seed(uint64_t* seed)
{
    uint64_t mix = (*seed += MIX_CONSTANT_1);
    mix = (mix ^ (mix >> 30)) * MIX_CONSTANT_2;
    mix = (mix ^ (mix >> 27)) * MIX_CONSTANT_3;
    return mix ^ (mix >> 31);
}

static uint64_t ranbo_detail_xoshiro256ss_rotate(uint64_t number,
                                                 int bits_to_rotate)
{
    return (number << bits_to_rotate) | (number >> (64 - bits_to_rotate));
}

static void ranbo_xoshiro256ss_set_seed(struct ranbo_xoshiro256ss* generator,
                                        uint64_t seed)
{
    generator->seed[0] = ranbo_detail_xoshiro256ss_split_and_mix_seed(&seed);
    generator->seed[1] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(generator->seed[0]));
    generator->seed[2] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(generator->seed[1]));
    generator->seed[3] =
        ranbo_detail_xoshiro256ss_split_and_mix_seed(&(generator->seed[2]));
}

static uint64_t
ranbo_xoshiro256ss_generate(struct ranbo_xoshiro256ss* generator)
{
    uint64_t result =
        ranbo_detail_xoshiro256ss_rotate(generator->seed[1] * 5, 7) * 9;
    uint64_t temp = generator->seed[1] << 17;
    generator->seed[2] ^= generator->seed[0];
    generator->seed[3] ^= generator->seed[1];
    generator->seed[1] ^= generator->seed[2];
    generator->seed[0] ^= generator->seed[3];
    generator->seed[2] ^= temp;
    generator->seed[3] =
        ranbo_detail_xoshiro256ss_rotate(generator->seed[3], 45);
    return result;
}
