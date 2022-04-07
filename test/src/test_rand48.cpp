#include <ranbo/rand48.h>

#include <gtest/gtest.h>

TEST(test_rand48, api)
{
    struct ranbo_rand48 state;

    EXPECT_EQ(state.seed[0], 13070U);
    EXPECT_EQ(state.seed[1], 27840U);
    EXPECT_EQ(state.seed[2], 31222U);

    uint32_t random_number = ranbo_rand48_generate(&state);
    EXPECT_EQ(random_number, 1079783580U);

    random_number = ranbo_rand48_generate(&state);
    EXPECT_EQ(random_number, 776711745U);

    ranbo_rand48_set_seed(&state, 0U);

    EXPECT_EQ(state.seed[0], 13070U);
    EXPECT_EQ(state.seed[1], 0U);
    EXPECT_EQ(state.seed[2], 0U);
}
