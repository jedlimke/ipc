#ifndef RX_H
#define RX_H

#include <string>
#include <optional> // Include optional
#include "ipc_data.pb.h"  // Include the Protobuf header

// Define a plain data structure for processed IPCData
struct T_IPCData {
    std::optional<int>           theInt;    // Nullable int
    std::optional<float>         theFloat;  // Nullable float
    std::optional<std::string>   theString; // Nullable string
    std::optional<IPCData::Type> theType;   // Nullable enum
};

/**
 * @brief Handles a serialized IPCData message.
 *
 * This function takes a serialized Protobuf message as input, deserializes it
 * into an IPCData object, processes the data, and returns a string indicating
 * the result of the processing.
 *
 * @param serializedMessage A serialized Protobuf message (IPCData).
 * @return A T_IPCData structure representing the processed data.
 * @throws std::runtime_error if deserialization fails or data is invalid.
 */
T_IPCData handleRxMessage(const std::string& serializedMessage);

#endif // RX_H