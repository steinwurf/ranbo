#include <ranbo/xoshiro64s.h>

#include <gtest/gtest.h>

TEST(test_xoshiro64s, api)
{
    struct ranbo_xoshiro64s generator;

    EXPECT_EQ(generator.seed, 2203318231040U);
    EXPECT_EQ(generator.seed_parts[0], 513U);
    EXPECT_EQ(generator.seed_parts[1], 8192U);

    ranbo_xoshiro64s_set_seed(&generator, 1U);
    EXPECT_EQ(generator.seed, 1U);
    EXPECT_EQ(generator.seed_parts[0], 0U);
    EXPECT_EQ(generator.seed_parts[1], 1U);

    uint32_t random_number = ranbo_xoshiro64s_generate(&generator);
    EXPECT_EQ(random_number, 0U);
    EXPECT_EQ(generator.seed, 2203318231040U);

    random_number = ranbo_xoshiro64s_generate(&generator);
    EXPECT_EQ(random_number, 220917691U);

    ranbo_xoshiro64s_set_seed(&generator, 2203318231040U);
    EXPECT_EQ(generator.seed, 2203318231040U);
    EXPECT_EQ(generator.seed_parts[0], 513U);
    EXPECT_EQ(generator.seed_parts[1], 8192U);
}
