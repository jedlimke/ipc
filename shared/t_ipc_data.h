#ifndef T_IPC_DATA_H
#define T_IPC_DATA_H

#include <optional>
#include <string>
#include "ipc_data.pb.h"

class T_IPCData {
public:
    /**
     * @brief Default constructor.
     */
    T_IPCData();

    /**
     * @brief Constructor from a serialized string.
     * 
     * Initializes the object by deserializing the provided string.
     * 
     * @param serializedMessage A string representing the serialized data.
     * @throws std::runtime_error if deserialization fails.
     */
    T_IPCData(const std::string& serializedMessage);

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
    T_IPCData(std::optional<int> theInt,
              std::optional<float> theFloat,
              std::optional<std::string> theString,
              std::optional<IPCData::Type> theType);

    /**
     * @brief Converts the object into a human-readable string.
     * 
     * Creates a string representation of the current state of the object,
     * including all fields and their values.
     * 
     * @return A string containing the object's data in a readable format.
     */
    std::string ToString() const;

    /**
     * @brief Serializes the object into a string.
     * 
     * @return A string representing the serialized data.
     * @throws std::runtime_error if serialization fails.
     */
    std::string Serialize() const;

    /**
     * @brief Populates the object from a serialized string.
     * 
     * @param serializedMessage A string representing the serialized data.
     * @throws std::runtime_error if deserialization fails.
     */
    void Deserialize(const std::string& serializedMessage);

    // Getters
    std::optional<int> GetTheInt() const;
    std::optional<float> GetTheFloat() const;
    std::optional<std::string> GetTheString() const;
    std::optional<IPCData::Type> GetTheType() const;

    // Setters
    void SetTheInt(std::optional<int> theInt);
    void SetTheFloat(std::optional<float> theFloat);
    void SetTheString(std::optional<std::string> theString);
    void SetTheType(std::optional<IPCData::Type> theType);

private:
    std::optional<int> theInt_;
    std::optional<float> theFloat_;
    std::optional<std::string> theString_;
    std::optional<IPCData::Type> theType_;

    IPCData ToProtobuf() const;
    static IPCData FromProtobuf(const std::string& serializedMessage);
};

#endif // T_IPC_DATA_H
