// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <template/some.hpp>
#include <gtest/gtest.h>

TEST(test_some, return_value_of_some_method)
{
    template::some s;
    EXPECT_TRUE(s.some_method());
}
