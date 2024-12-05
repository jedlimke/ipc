#include <gtest/gtest.h>
#include "../include/rx.h"

TEST(RxTests, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(10, 20), 30);
}

TEST(RxTests, AddNegativeNumbers) {
    EXPECT_EQ(add(-1, -1), -2);
    EXPECT_EQ(add(-5, -10), -15);
}

TEST(RxTests, AddMixedNumbers) {
    EXPECT_EQ(add(-2, 5), 3);
    EXPECT_EQ(add(10, -20), -10);
}
