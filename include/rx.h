#ifndef RX_H
#define RX_H

#include "../include/ipc_data.h"
#include <string>
#include <stdexcept>

// Deserialize a serialized message into an IPCData structure
IPCData deserialize(const std::string& input);

#endif // RX_H
