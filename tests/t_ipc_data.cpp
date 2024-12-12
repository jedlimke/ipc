#include <gtest/gtest.h>
#include <limits>
#include "t_ipc_data.h"

/**
 * @brief Tests construction of a T_IPCData object with an empty string.
 * 
 * Verifies that attempting to construct an object with an empty string does NOT throw an exception.
 */
TEST(TIPCDataTests, ConstructFromEmptyString) {
    // Arrange: Create an empty serialized string
    std::string emptyMessage;

    // Act & Assert: Ensure construction does NOT throw an exception
    EXPECT_NO_THROW(
        T_IPCData newDataObject { emptyMessage }
    );
}

/**
 * @brief Tests equality of two T_IPCData objects after "serialization."
 * 
 * Verifies that an object constructed with one set of fields
 * can produce a new object with the same data through construction.
 */
TEST(TIPCDataTests, EqualObjects_AllFieldsSet) {
    // Arrange: Create the original data object
    T_IPCData originalDataObject(
        std::make_optional(47),
        std::make_optional(1701.0f),
        std::make_optional<std::string>("Make it so"),
        std::make_optional(IPCData::TYPE3)
    );

    // Act: Create a new data object from the serialized form of the original
    std::string serializedMessage = originalDataObject.Serialize();
    T_IPCData newDataObject { serializedMessage };

    // Assert: Ensure the new object matches the original
    EXPECT_EQ(newDataObject.GetTheInt().value(), originalDataObject.GetTheInt().value());
    EXPECT_FLOAT_EQ(newDataObject.GetTheFloat().value(), originalDataObject.GetTheFloat().value());
    EXPECT_EQ(newDataObject.GetTheString().value(), originalDataObject.GetTheString().value());
    EXPECT_EQ(newDataObject.GetTheType().value(), originalDataObject.GetTheType().value());
}

/**
 * @brief Tests equality of two T_IPCData objects with unset fields.
 * 
 * Verifies that unset fields remain unset in the new object.
 */
TEST(TIPCDataTests, EqualObjects_UnsetFields) {
    // Arrange: Create the original data object with some unset fields
    T_IPCData originalDataObject(
        std::nullopt,                       // Int not set
        std::make_optional(13.37f),         // Float set
        std::nullopt,                       // String not set
        std::make_optional(IPCData::TYPE1)  // Enum set
    );

    // Act: Create a new data object from the serialized form of the original
    std::string serializedMessage = originalDataObject.Serialize();
    T_IPCData newDataObject { serializedMessage };

    // Assert: Ensure the new object matches the original
    EXPECT_EQ(newDataObject.GetTheInt().has_value(), originalDataObject.GetTheInt().has_value());
    EXPECT_FLOAT_EQ(newDataObject.GetTheFloat().value(), originalDataObject.GetTheFloat().value());
    EXPECT_EQ(newDataObject.GetTheString().has_value(), originalDataObject.GetTheString().has_value());
    EXPECT_EQ(newDataObject.GetTheType().value(), originalDataObject.GetTheType().value());
}

/**
 * @brief Tests equality of two T_IPCData objects with default values.
 * 
 * Verifies that a new object with default values matches the original.
 */
TEST(TIPCDataTests, EqualObjects_DefaultValues) {
    // Arrange: Create the original data object with default fields
    T_IPCData originalDataObject;

    // Act: Create a new data object from the serialized form of the original
    std::string serializedMessage = originalDataObject.Serialize();
    T_IPCData newDataObject { serializedMessage };

    // Assert: Ensure the new object matches the original
    EXPECT_EQ(newDataObject.GetTheInt().has_value(), originalDataObject.GetTheInt().has_value());
    EXPECT_EQ(newDataObject.GetTheFloat().has_value(), originalDataObject.GetTheFloat().has_value());
    EXPECT_EQ(newDataObject.GetTheString().has_value(), originalDataObject.GetTheString().has_value());
    EXPECT_EQ(newDataObject.GetTheType().has_value(), originalDataObject.GetTheType().has_value());
}

/**
 * @brief Tests equality of two T_IPCData objects with specific values.
 * 
 * Validates the correctness of construction and comparison for known values.
 */
TEST(TIPCDataTests, EqualObjects_SpecificValues) {
    // Arrange: Create the original data object with specific fields
    T_IPCData originalDataObject(
        std::make_optional(48151623),
        std::make_optional(42.0f),
        std::make_optional<std::string>("Not Penny's Boat"),
        std::make_optional(IPCData::TYPE2)
    );

    // Act: Create a new data object from the serialized form of the original
    std::string serializedMessage = originalDataObject.Serialize();
    T_IPCData newDataObject { serializedMessage };

    // Assert: Ensure the new object matches the original
    EXPECT_EQ(newDataObject.GetTheInt().value(), originalDataObject.GetTheInt().value());
    EXPECT_FLOAT_EQ(newDataObject.GetTheFloat().value(), originalDataObject.GetTheFloat().value());
    EXPECT_EQ(newDataObject.GetTheString().value(), originalDataObject.GetTheString().value());
    EXPECT_EQ(newDataObject.GetTheType().value(), originalDataObject.GetTheType().value());
}

/**
 * @brief Tests the handling of INT_MIN as the integer field.
 * 
 * Verifies that T_IPCData correctly handles the smallest possible integer value.
 */
TEST(TIPCDataTests, HandleIntMin) {
    // Arrange: Create a T_IPCData object with INT_MIN
    T_IPCData data(
        std::make_optional(std::numeric_limits<int>::min()), // INT_MIN
        std::nullopt,
        std::nullopt,
        std::nullopt
    );

    // Act: Serialize and then deserialize the object
    std::string serializedMessage = data.Serialize();
    T_IPCData deserializedData { serializedMessage };

    // Assert: Ensure deserialized data matches the original
    EXPECT_EQ(deserializedData.GetTheInt().value(), std::numeric_limits<int>::min());
}

/**
 * @brief Tests the handling of INT_MAX as the integer field.
 * 
 * Verifies that T_IPCData correctly handles the largest possible integer value.
 */
TEST(TIPCDataTests, HandleIntMax) {
    // Arrange: Create a T_IPCData object with INT_MAX
    T_IPCData data(
        std::make_optional(std::numeric_limits<int>::max()), // INT_MAX
        std::nullopt,
        std::nullopt,
        std::nullopt
    );

    // Act: Serialize and then deserialize the object
    std::string serializedMessage = data.Serialize();
    T_IPCData deserializedData { serializedMessage };

    // Assert: Ensure deserialized data matches the original
    EXPECT_EQ(deserializedData.GetTheInt().value(), std::numeric_limits<int>::max());
}

/**
 * @brief Tests the handling of the largest possible float value.
 * 
 * Verifies that T_IPCData correctly handles `std::numeric_limits<float>::max()`.
 */
TEST(TIPCDataTests, HandleFloatMax) {
    // Arrange: Create a T_IPCData object with the largest possible float
    T_IPCData data(
        std::nullopt,
        std::make_optional(std::numeric_limits<float>::max()),
        std::nullopt,
        std::nullopt
    );

    // Act: Serialize and then deserialize the object
    std::string serializedMessage = data.Serialize();
    T_IPCData deserializedData { serializedMessage };

    // Assert: Ensure deserialized data matches the original
    EXPECT_FLOAT_EQ(deserializedData.GetTheFloat().value(), std::numeric_limits<float>::max());
}

/**
 * @brief Tests the handling of the smallest possible float value.
 * 
 * Verifies that T_IPCData correctly handles `std::numeric_limits<float>::lowest()`.
 */
TEST(TIPCDataTests, HandleFloatMin) {
    // Arrange: Create a T_IPCData object with the smallest possible float
    T_IPCData data(
        std::nullopt,
        std::make_optional(std::numeric_limits<float>::lowest()),
        std::nullopt,
        std::nullopt
    );

    // Act: Serialize and then deserialize the object
    std::string serializedMessage = data.Serialize();
    auto deserializedData = T_IPCData { serializedMessage };

    // Assert: Ensure deserialized data matches the original
    EXPECT_FLOAT_EQ(deserializedData.GetTheFloat().value(), std::numeric_limits<float>::lowest());
}

/**
 * @brief Tests handling of invalid enum values in deserialized data.
 * 
 * White-box viewpoint, simulates a situation where an out-of-range enum value is received.
 */
TEST(TIPCDataTests, HandleInvalidEnum) {
    // Arrange: Create a Protobuf message with an invalid enum value
    IPCData proto;
    proto.set_the_type(static_cast<IPCData::Type>(999));
    std::string serializedMessage;
    ASSERT_TRUE(proto.SerializeToString(&serializedMessage));

    // Act & Assert: Expect construction to throw an exception
    EXPECT_THROW(
        T_IPCData data { serializedMessage },
        std::out_of_range
    );
}

/**
 * @brief Failing test on purpose to test GitHub Actions
 */
TEST(TIPCDataTests, FailureFailureFailureFailure) {
    EXPECT_EQ(1, 2); // Intentional failure
}