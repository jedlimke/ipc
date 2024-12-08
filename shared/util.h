#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util {
    /**
     * @brief Gets the current date and time down to milliseconds as a string.
     * 
     * This function retrieves the current system date and time formatted as:
     * "YYYY-MM-DD HH:MM:SS.mmm".
     * 
     * @return A string representation of the current date and time, including milliseconds.
     */
    std::string getCurrentDateTimeWithMilliseconds();
}

#endif // UTIL_H
