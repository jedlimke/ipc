#include <gtest/gtest.h>
#include "../include/rx.h"

// Test adding two positive numbers
TEST(RxTests, AddPositiveNumbers) {
    // Arrange: Set up the inputs
    int a = 2;
    int b = 3;

    // Act: Perform the operation
    int result = add(a, b);

    // Assert: Verify the result
    EXPECT_EQ(result, 5);
}

// Test adding two negative numbers
TEST(RxTests, AddNegativeNumbers) {
    // Arrange: Set up the inputs
    int a = -1;
    int b = -4;

    // Act: Perform the operation
    int result = add(a, b);

    // Assert: Verify the result
    EXPECT_EQ(result, -5);
}

// Test adding a positive and a negative number
TEST(RxTests, AddMixedNumbers) {
    // Arrange: Set up the inputs
    int a = 10;
    int b = -3;

    // Act: Perform the operation
    int result = add(a, b);

    // Assert: Verify the result
    EXPECT_EQ(result, 7);
}

// Test adding zero
TEST(RxTests, AddZero) {
    // Arrange: Set up the inputs
    int a = 42;
    int b = 0;

    // Act: Perform the operation
    int result = add(a, b);

    // Assert: Verify the result
    EXPECT_EQ(result, 42);
}

// Test adding two zeros
TEST(RxTests, AddTwoZeros) {
    // Arrange: Set up the inputs
    int a = 0;
    int b = 0;

    // Act: Perform the operation
    int result = add(a, b);

    // Assert: Verify the result
    EXPECT_EQ(result, 0);
}
