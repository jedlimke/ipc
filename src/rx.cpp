#include "../include/rx.h"
#include <sstream>
#include <iostream>
#include <algorithm>  // Need this for std::count to work

// Helper function to unescape strings (e.g., "\|" becomes "|")
std::string unescapeString(const std::string& input) {
    std::string output;
    bool escape = false;
    for (char ch : input) {
        if (escape) {
            output += ch;  // Add escaped character
            escape = false;
        } else if (ch == '\\') {
            escape = true;  // Next character is escaped
        } else {
            output += ch;  // Add normal character
        }
    }
    return output;
}

// Helper function to validate we've got the right number of delimiters
void checkDelimeterCount(const std::string& input) {
    const int expectedFields = 4;  // ID, float, string, enum
    int delimiterCount = 0;
    bool escape = false;

    for (char ch : input) {
        if (escape) {
            escape = false;  // Skip escaped characters
        } else if (ch == '\\') {
            escape = true;  // Next character is escaped
        } else if (ch == '|') {
            delimiterCount++;  // Count unescaped delimiters
        }
    }

    if (delimiterCount != expectedFields - 1) {
        throw std::runtime_error("Invalid message: incorrect number of fields in input");
    }
}

// Deserialize a serialized message into an IPCData structure
IPCData deserialize(const std::string& input) {
    // This'll blow up if we don't have the correct number of fields--and it should! Fail HARD!
    checkDelimeterCount(input);

    // Unescape and split input
    std::stringstream ss(unescapeString(input));
    std::string token;

    IPCData data;

    // Parse fields directly
    std::getline(ss, token, '|');
    data.theInt = std::stoi(token);

    std::getline(ss, token, '|');
    data.theFloat = std::stof(token);

    std::getline(ss, token, '|');
    data.theString = token;

    std::getline(ss, token, '|');
    int typeValue = std::stoi(token);
    if (typeValue < 0 || typeValue > 2) {
        throw std::runtime_error("Invalid message: unknown enum type");
    }
    data.theType = static_cast<IPCData::Type>(typeValue);

    return data;
}
