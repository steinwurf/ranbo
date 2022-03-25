#include <ranbo/xoshiro64s.h>

#include <gtest/gtest.h>

TEST(test_xoshiro64s, api)
{
    struct ranbo_xoshiro64s generator;

    EXPECT_EQ(generator.seed, 1U);
    EXPECT_EQ(generator.seed_parts[0], 3793791033U);
    EXPECT_EQ(generator.seed_parts[1], 2065550767U);

    uint32_t random_number = ranbo_xoshiro64s_generate(&generator);
    EXPECT_EQ(random_number, 4020032163U);
    EXPECT_EQ(generator.seed, 12037577613245804387U);

    random_number = ranbo_xoshiro64s_generate(&generator);
    EXPECT_EQ(random_number, 2576321781U);

    ranbo_xoshiro64s_set_seed(&generator, 2203318231040U);
    EXPECT_EQ(generator.seed, 2203318231040U);
    EXPECT_EQ(generator.seed_parts[0], 3793791033U);
    EXPECT_EQ(generator.seed_parts[1], 2065550767U);
}
