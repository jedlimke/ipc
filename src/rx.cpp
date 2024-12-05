#include "../include/rx.h"
#include <sstream>
#include <iostream>

// Helper function to unescape strings (e.g., "\|" becomes "|")
std::string unescapeString(const std::string& input) {
    std::string output;
    bool escape = false;
    for (char ch : input) {
        if (escape) {
            output += ch;
            escape = false;
        } else if (ch == '\\') {
            escape = true;
        } else {
            output += ch;
        }
    }
    return output;
}

// Deserialize a serialized message into an IPCData structure
IPCData deserialize(const std::string& input) {
    // Split the string by the '|' delimiter
    std::stringstream ss(input);
    std::string token;

    IPCData data;

    // Parse the integer ID
    if (std::getline(ss, token, '|')) {
        data.theInt = std::stoi(token);
    } else {
        throw std::runtime_error("Invalid message: missing ID");
    }

    // Parse the float value
    if (std::getline(ss, token, '|')) {
        data.theFloat = std::stof(token);
    } else {
        throw std::runtime_error("Invalid message: missing float value");
    }

    // Parse the escaped string
    if (std::getline(ss, token, '|')) {
        data.theString = unescapeString(token);
    } else {
        throw std::runtime_error("Invalid message: missing text");
    }

    // Parse the enum type
    if (std::getline(ss, token, '|')) {
        int typeValue = std::stoi(token);
        if (typeValue < 0 || typeValue > 2) {
            throw std::runtime_error("Invalid message: unknown enum type");
        }
        data.theType = static_cast<IPCData::Type>(typeValue);
    } else {
        throw std::runtime_error("Invalid message: missing enum type");
    }

    return data;
}
