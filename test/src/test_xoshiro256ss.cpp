
#include <ranbo/xoshiro256ss.h>

#include <gtest/gtest.h>

TEST(test_xoshiro256ss, api)
{
    struct ranbo_xoshiro256ss generator;

    ranbo_xoshiro256ss_set_seed(&generator, 0U);

    EXPECT_EQ(generator.seed[0], 9248179162272254404U);
    EXPECT_EQ(generator.seed[1], 4989520995034593924U);
    EXPECT_EQ(generator.seed[2], 13959451808893450918U);
    EXPECT_EQ(generator.seed[3], 2391539541053276776U);

    uint64_t random_number = ranbo_xoshiro256ss_generate(&generator);
    EXPECT_EQ(random_number, 18060408633489136533U);

    EXPECT_EQ(generator.seed[0], 16453342208284930344U);
    EXPECT_EQ(generator.seed[1], 351166900854360550U);
    EXPECT_EQ(generator.seed[2], 17019197827783816034U);
    EXPECT_EQ(generator.seed[3], 9159631706113461461U);

    ranbo_xoshiro256ss_set_seed(&generator, 1U);

    EXPECT_EQ(generator.seed[0], 3405187124814469334U);
    EXPECT_EQ(generator.seed[1], 18192612585172622643U);
    EXPECT_EQ(generator.seed[2], 5747011685946229464U);
    EXPECT_EQ(generator.seed[3], 16321491304643971414U);
}
