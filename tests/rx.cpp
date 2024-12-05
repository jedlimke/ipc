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

// Test handling of a message with non-numeric values in numeric fields
TEST(RxTests, HandleNonNumericFields_badInt) {
    // Arrange: Define a serialized message with a non-numeric int value
    std::string input = "NotAnythingWeCanParseToInt|1.2|Valid string|1";

    // Act & Assert: Expect std::invalid_argument
    EXPECT_THROW({
        IPCData data = deserialize(input);
    }, std::invalid_argument);
}

// Test handling of a message with non-numeric values in numeric fields
TEST(RxTests, HandleNonNumericFields_badFloat) {
    // Arrange: Define a serialized message with a non-numeric float value
    std::string input = "5|NotAnythingWeCanParseToFloat|Valid string|1";

    // Act & Assert: Expect std::invalid_argument
    EXPECT_THROW({
        IPCData data = deserialize(input);
    }, std::invalid_argument);
}

// Test deserialization of a valid message that happens to have funky characters in the string
TEST(RxTests, ParseValidMessageWithFunkyString) {
    // Arrange: Define a serialized message with funky characters
    std::string input = "2|42.42|This is a test with \\| delimiters and \\\\slashes|2";

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

// Test handling of a message with too few delimiters
TEST(RxTests, HandleTooFewDelimiters) {
    // Arrange: Define a serialized message short on delimiters
    std::string input = "3|100.0";

    // Act & Assert: Ensure an exception is thrown with the correct error message
    try {
        IPCData data = deserialize(input);
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(e.what(), std::string("Invalid message: incorrect number of fields in input"));
    } catch (...) {
        FAIL() << "Expected std::runtime_error, but got a different exception";
    }
}

// Test handling of a message with too many delimiters
TEST(RxTests, HandleTooManyDelimiters) {
    // Arrange: Define a serialized message with extra delimiters
    std::string input = "3|100.0|Extra|Delimiters|Are|Invalid|2";

    // Act & Assert: Ensure an exception is thrown with the correct error message
    try {
        IPCData data = deserialize(input);
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(e.what(), std::string("Invalid message: incorrect number of fields in input"));
    } catch (...) {
        FAIL() << "Expected std::runtime_error, but got a different exception";
    }
}
