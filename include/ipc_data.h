#ifndef IPC_DATA_H
#define IPC_DATA_H

#include <string>

// Shared data structure for Tx and Rx
struct IPCData {
    int theInt;             // Integer ID
    float theFloat;         // Floating-point value
    std::string theString;  // Text message
    enum Type { TYPE1 = 0, TYPE2 = 1, TYPE3 = 2 } theType;
};

#endif // IPC_DATA_H
