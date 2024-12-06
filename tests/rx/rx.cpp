#include <gtest/gtest.h>
#include "../../src/rx/rx.h"

/**
 * @brief Test handling of a valid serialized message with all fields populated.
 */
TEST(RxTests, HandleValidMessage) {
    // Arrange: Prepare a valid IPCData object and serialize it
    IPCData data;
    data.set_the_int(42);
    data.set_the_float(3.14f);
    data.set_the_string("Hello, Rx!");
    data.set_the_type(IPCData::TYPE2);

    std::string serialized;
    ASSERT_TRUE(data.SerializeToString(&serialized)) << "Failed to serialize IPCData.";

    // Act: Process the serialized message
    T_IPCData result = handleRxMessage(serialized);

    // Assert: Verify the returned T_IPCData structure
    ASSERT_TRUE(result.theInt.has_value());
    EXPECT_EQ(result.theInt.value(), 42);

    ASSERT_TRUE(result.theFloat.has_value());
    EXPECT_FLOAT_EQ(result.theFloat.value(), 3.14f);

    ASSERT_TRUE(result.theString.has_value());
    EXPECT_EQ(result.theString.value(), "Hello, Rx!");

    ASSERT_TRUE(result.theType.has_value());
    EXPECT_EQ(result.theType.value(), IPCData::TYPE2);
}

/**
 * @brief Test handling of an empty serialized message.
 */
TEST(RxTests, HandleEmptyMessage) {
    // Arrange: Prepare an empty IPCData object
    IPCData data;
    std::string serialized;
    ASSERT_TRUE(data.SerializeToString(&serialized)) << "Failed to serialize empty IPCData.";

    // Act: Process the serialized message
    T_IPCData result = handleRxMessage(serialized);

    // Assert: Verify that all fields in T_IPCData are std::nullopt
    EXPECT_FALSE(result.theInt.has_value());
    EXPECT_FALSE(result.theFloat.has_value());
    EXPECT_FALSE(result.theString.has_value());
    EXPECT_FALSE(result.theType.has_value());
}

/**
 * @brief Test handling of corrupted serialized input.
 */
TEST(RxTests, HandleCorruptedMessage) {
    // Arrange: Prepare an invalid serialized string
    std::string corruptedMessage = "invalid_data";

    // Act & Assert: Verify that the function throws an exception
    EXPECT_THROW({
        handleRxMessage(corruptedMessage);
    }, std::runtime_error);
}

/**
 * @brief Test handling of a partially populated serialized message.
 */
TEST(RxTests, HandlePartiallyPopulatedMessage) {
    // Arrange: Prepare a partially populated IPCData object
    IPCData data;
    data.set_the_int(123);  // Only this field is set
    std::string serialized;
    ASSERT_TRUE(data.SerializeToString(&serialized)) << "Failed to serialize partial IPCData.";

    // Act: Process the serialized message
    T_IPCData result = handleRxMessage(serialized);

    // Assert: Verify that only theInt is populated
    ASSERT_TRUE(result.theInt.has_value());
    EXPECT_EQ(result.theInt.value(), 123);

    EXPECT_FALSE(result.theFloat.has_value());
    EXPECT_FALSE(result.theString.has_value());
    EXPECT_FALSE(result.theType.has_value());
}

/**
 * @brief Test handling of valid serialized message with edge case values.
 */
TEST(RxTests, HandleMessageWithEdgeCaseValues) {
    // Arrange: Prepare an IPCData object with edge case values
    IPCData data;
    data.set_the_int(0);                // Edge case: zero integer
    data.set_the_float(-0.0f);          // Edge case: negative zero
    data.set_the_string("");            // Edge case: empty string
    data.set_the_type(IPCData::TYPE3);  // Edge case: highest enum value

    std::string serialized;
    ASSERT_TRUE(data.SerializeToString(&serialized)) << "Failed to serialize edge case IPCData.";

    // Act: Process the serialized message
    T_IPCData result = handleRxMessage(serialized);

    // Assert: Verify the edge case values
    ASSERT_TRUE(result.theInt.has_value());
    EXPECT_EQ(result.theInt.value(), 0);

    ASSERT_TRUE(result.theFloat.has_value());
    EXPECT_FLOAT_EQ(result.theFloat.value(), -0.0f);

    ASSERT_TRUE(result.theString.has_value());
    EXPECT_EQ(result.theString.value(), "");

    ASSERT_TRUE(result.theType.has_value());
    EXPECT_EQ(result.theType.value(), IPCData::TYPE3);
}
