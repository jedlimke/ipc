#include <gtest/gtest.h>
#include "../include/rx.h"

// Test deserialization of a valid message
TEST(RxTests, ParseValidMessage) {
    // Arrange: Define a valid serialized message
    std::string input = "1|3.14|Hello, IPC!|1";  // ID, float, string, enum

    // Act: Deserialize the message
    IPCData data = deserialize(input);

    // Assert: Verify the result matches the expected structure
    EXPECT_EQ(data.theInt, 1);
    EXPECT_FLOAT_EQ(data.theFloat, 3.14f);
    EXPECT_EQ(data.theString, "Hello, IPC!");
    EXPECT_EQ(data.theType, IPCData::TYPE2);  // Enum type 1 maps to TYPE2
}

// Test deserialization of a message with funky characters in the string
TEST(RxTests, ParseMessageWithFunkyString) {
    // Arrange: Define a serialized message with funky characters
    std::string input = "2|42.42|This is a test with | delimiters and \\slashes|2";

    // Act: Deserialize the message
    IPCData data = deserialize(input);

    // Assert: Verify the funky string is parsed correctly
    EXPECT_EQ(data.theInt, 2);
    EXPECT_FLOAT_EQ(data.theFloat, 42.42f);
    EXPECT_EQ(data.theString, "This is a test with | delimiters and \\slashes");
    EXPECT_EQ(data.theType, IPCData::TYPE3);
}

// Test handling of an invalid message
TEST(RxTests, HandleInvalidMessage) {
    // Arrange: Define an invalid serialized message (missing a field)
    std::string input = "3|100.0|Incomplete";

    // Act & Assert: Deserialize should throw an exception or indicate an error
    EXPECT_THROW(deserialize(input), std::runtime_error);
}