#include <gtest/gtest.h>
#include <Clarabel>


// Demonstrate some basic assertions.
TEST(BasicLP, Test1)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}