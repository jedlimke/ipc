#ifndef T_IPC_DATA_H
#define T_IPC_DATA_H

#include <optional>
#include <string>
#include "ipc_data.pb.h"

class T_IPCData {
public:
    // Constructors
    T_IPCData();
    T_IPCData(const std::string& serializedMessage);
    T_IPCData(std::optional<int> theInt,
              std::optional<float> theFloat,
              std::optional<std::string> theString,
              std::optional<IPCData::Type> theType);

    // Methods
    std::string ToString() const;
    std::string Serialize() const;

    // Getters
    std::optional<int> GetTheInt() const;
    std::optional<float> GetTheFloat() const;
    std::optional<std::string> GetTheString() const;
    std::optional<IPCData::Type> GetTheType() const;

private:
    // Members
    std::optional<int> theInt_;
    std::optional<float> theFloat_;
    std::optional<std::string> theString_;
    std::optional<IPCData::Type> theType_;

    // Helper methods
    IPCData ToProtobuf() const;
    static IPCData FromProtobuf(const std::string& serializedMessage);
};

#endif // T_IPC_DATA_H
