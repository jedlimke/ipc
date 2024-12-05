#include "rx.h"
#include <optional> // Include optional
#include <stdexcept>

T_IPCData handleRxMessage(const std::string& serializedMessage) {
    IPCData data;

    // Attempt to deserialize the message
    if (!data.ParseFromString(serializedMessage)) {
        throw std::runtime_error("Failed to parse IPCData message.");
    }

    // Populate the T_IPCData structure, checking for field presence
    T_IPCData result;
    result.theInt    = data.has_the_int()    ? std::optional<int>(data.the_int())            : std::nullopt;
    result.theFloat  = data.has_the_float()  ? std::optional<float>(data.the_float())        : std::nullopt;
    result.theString = data.has_the_string() ? std::optional<std::string>(data.the_string()) : std::nullopt;
    result.theType   = data.has_the_type()   ? std::optional<IPCData::Type>(data.the_type()) : std::nullopt;

    return result;  // Return the structured data
}