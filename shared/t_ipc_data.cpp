#include <optional>
#include <string>
#include <sstream>
#include <stdexcept>
#include "t_ipc_data.h"

/**
 * @brief Default constructor.
 * 
 * Initializes all fields to unset (nullopt).
 */
T_IPCData::T_IPCData()
    : theInt_(std::nullopt), theFloat_(std::nullopt), theString_(std::nullopt), theType_(std::nullopt) { }

/**
 * @brief Constructor from a serialized string.
 * 
 * Initializes the object by deserializing the provided string.
 * 
 * @param serializedMessage A string representing the serialized data.
 * @throws std::runtime_error if deserialization fails.
 */
T_IPCData::T_IPCData(const std::string& serializedMessage) {
    IPCData proto = FromProtobuf(serializedMessage);

    theInt_    = proto.has_the_int()    ? std::optional<int>(proto.the_int())            : std::nullopt;
    theFloat_  = proto.has_the_float()  ? std::optional<float>(proto.the_float())        : std::nullopt;
    theString_ = proto.has_the_string() ? std::optional<std::string>(proto.the_string()) : std::nullopt;
    theType_   = proto.has_the_type()   ? std::optional<IPCData::Type>(proto.the_type()) : std::nullopt;
}

/**
 * @brief Constructor from individual fields.
 * 
 * Initializes the object with the provided values.
 * 
 * @param theInt Optional integer value.
 * @param theFloat Optional floating-point value.
 * @param theString Optional string value.
 * @param theType Optional enum value.
 */
T_IPCData::T_IPCData(std::optional<int> theInt,
                     std::optional<float> theFloat,
                     std::optional<std::string> theString,
                     std::optional<IPCData::Type> theType)
    : theInt_(theInt), theFloat_(theFloat), theString_(theString), theType_(theType) { }

/**
 * @brief Serializes the object into a string.
 */
std::string T_IPCData::Serialize() const {
    IPCData proto = ToProtobuf();

    std::string serialized;
    if (!proto.SerializeToString(&serialized)) {
        throw std::runtime_error("Failed to serialize T_IPCData.");
    }

    return serialized;
}

/**
 * @brief Converts the object into a human-readable string.
 */
std::string T_IPCData::ToString() const {
    std::ostringstream oss;

    oss << "  Int:    " << (theInt_    ? std::to_string(*theInt_)                    : "Not set") << '\n';
    oss << "  Float:  " << (theFloat_  ? std::to_string(*theFloat_)                  : "Not set") << '\n';
    oss << "  String: " << (theString_ ? *theString_                                 : "Not set") << '\n';
    oss << "  Type:   " << (theType_   ? std::to_string(static_cast<int>(*theType_)) : "Not set") << '\n';

    return oss.str();
}

// Getters
std::optional<int> T_IPCData::GetTheInt() const { return theInt_; }
std::optional<float> T_IPCData::GetTheFloat() const { return theFloat_; }
std::optional<std::string> T_IPCData::GetTheString() const { return theString_; }
std::optional<IPCData::Type> T_IPCData::GetTheType() const { return theType_; }

/* -----------------------------------------
   Private Helper Methods
   ----------------------------------------- */

IPCData T_IPCData::ToProtobuf() const {
    IPCData proto;

    if (theInt_.has_value())    proto.set_the_int(theInt_.value());
    if (theFloat_.has_value())  proto.set_the_float(theFloat_.value());
    if (theString_.has_value()) proto.set_the_string(theString_.value());
    if (theType_.has_value())   proto.set_the_type(theType_.value());

    return proto;
}

/**
 * @brief Creates a Protobuf object from serialized data.
 * 
 * Converts a serialized string into a Protobuf object for internal use.
 * 
 * @param serializedMessage A string representing the serialized data.
 * @return A Protobuf object containing the deserialized data.
 * @throws std::runtime_error if parsing fails or the data is invalid.
 */
IPCData T_IPCData::FromProtobuf(const std::string& serializedMessage) {
    IPCData proto;
    if (!proto.ParseFromString(serializedMessage)) {
        throw std::runtime_error("Failed to parse IPCData message.");
    }

    // Validate enum field
    if (proto.has_the_type()) {
        int enumValue = proto.the_type();
        if (enumValue < IPCData::TYPE1 || enumValue > IPCData::TYPE3) {
            throw std::out_of_range("Enum value for 'theType' in IPCData is out of range.");
        }
    }

    return proto;
}
