#include <gtest/gtest.h>
#include "t_ipc_data.h"

/**
 * @brief Tests the serialization of a T_IPCData object with all fields set.
 * 
 * Verifies that a T_IPCData object containing valid values can be serialized
 * and deserialized without losing data.
 */
TEST(TIPCDataTests, SerializeAndDeserializeAllFields) {
    // Arrange: Create a T_IPCData object with all fields set
    T_IPCData data(
        std::make_optional(47),
        std::make_optional(1701.0f),
        std::make_optional<std::string>("Make it so"),
        std::make_optional(IPCData::TYPE3)
    );

    // Act: Serialize and then deserialize the object
    std::string serialized = data.Serialize();
    T_IPCData deserializedData(serialized);

    // Assert: Ensure the deserialized data matches the original
    EXPECT_EQ(deserializedData.GetTheInt().value(), 47);
    EXPECT_FLOAT_EQ(deserializedData.GetTheFloat().value(), 1701.0f);
    EXPECT_EQ(deserializedData.GetTheString().value(), "Make it so");
    EXPECT_EQ(deserializedData.GetTheType().value(), IPCData::TYPE3);
}

/**
 * @brief Tests the serialization and deserialization of a T_IPCData object with unset fields.
 * 
 * Verifies that unset fields remain unset after serialization and deserialization.
 */
TEST(TIPCDataTests, SerializeAndDeserializeUnsetFields) {
    // Arrange: Create a T_IPCData object with only some fields set
    T_IPCData data(
        std::nullopt,                       // Int not set
        std::make_optional(13.37f),         // Float set
        std::nullopt,                       // String not set
        std::make_optional(IPCData::TYPE1)  // Enum set
    );

    // Act: Serialize and then deserialize the object
    std::string serialized = data.Serialize();
    T_IPCData deserializedData(serialized);

    // Assert: Ensure fields retain their correct state
    EXPECT_FALSE(deserializedData.GetTheInt().has_value());
    EXPECT_FLOAT_EQ(deserializedData.GetTheFloat().value(), 13.37f);
    EXPECT_FALSE(deserializedData.GetTheString().has_value());
    EXPECT_EQ(deserializedData.GetTheType().value(), IPCData::TYPE1);
}

/**
 * @brief Tests the deserialization of an empty string.
 * 
 * Verifies that attempting to deserialize an empty string does NOT throw an exception.
 */
TEST(TIPCDataTests, DeserializeEmptyString) {
    // Arrange: Create an empty serialized string
    std::string emptyMessage;

    // Act & Assert: Ensure deserialization does NOT throw an exception
    EXPECT_NO_THROW(
        T_IPCData data(emptyMessage)
    );
}

/**
 * @brief Tests the serialization of a T_IPCData object with default values.
 * 
 * Verifies that a T_IPCData object with default values can be serialized
 * and deserialized correctly.
 */
TEST(TIPCDataTests, SerializeAndDeserializeDefaultValues) {
    // Arrange: Create a T_IPCData object with all default fields
    T_IPCData data;

    // Act: Serialize and then deserialize the object
    std::string serialized = data.Serialize();
    T_IPCData deserializedData(serialized);

    // Assert: Ensure all fields are unset
    EXPECT_FALSE(deserializedData.GetTheInt().has_value());
    EXPECT_FALSE(deserializedData.GetTheFloat().has_value());
    EXPECT_FALSE(deserializedData.GetTheString().has_value());
    EXPECT_FALSE(deserializedData.GetTheType().has_value());
}

/**
 * @brief Tests the serialization of a T_IPCData object.
 * 
 * Validates the correctness of serialization and
 * deserialization for known values.
 */
TEST(TIPCDataTests, SerializeAndDeserializeValues) {
    // Arrange: Create a T_IPCData object all fields set
    T_IPCData data(
        std::make_optional(48151623),
        std::make_optional(42.0f),
        std::make_optional<std::string>("Not Penny's Boat"),
        std::make_optional(IPCData::TYPE2)
    );

    // Act: Serialize and then deserialize the object
    std::string serialized = data.Serialize();
    T_IPCData deserializedData(serialized);

    // Assert: Ensure fields retain their correct values
    EXPECT_EQ(deserializedData.GetTheInt().value(), 48151623);
    EXPECT_FLOAT_EQ(deserializedData.GetTheFloat().value(), 42.0f);
    EXPECT_EQ(deserializedData.GetTheString().value(), "Not Penny's Boat");
    EXPECT_EQ(deserializedData.GetTheType().value(), IPCData::TYPE2);
}