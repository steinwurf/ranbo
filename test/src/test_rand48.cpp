#include <ranbo/rand48.h>

#include <gtest/gtest.h>

TEST(test_rand48, api)
{
    struct ranbo_rand48 state;

    ranbo_rand48_set_seed(&state, 0U);

    EXPECT_EQ(state.seed[0], 13070U);
    EXPECT_EQ(state.seed[1], 0U);
    EXPECT_EQ(state.seed[2], 0U);

    uint32_t random_number = ranbo_rand48_generate(&state);
    EXPECT_EQ(random_number, 733700828U);

    random_number = ranbo_rand48_generate(&state);
    EXPECT_EQ(random_number, 3220804481U);

    ranbo_rand48_set_seed(&state, 0U);

    EXPECT_EQ(state.seed[0], 13070U);
    EXPECT_EQ(state.seed[1], 0U);
    EXPECT_EQ(state.seed[2], 0U);
}
