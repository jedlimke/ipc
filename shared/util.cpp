#include "util.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace util {
    std::string getCurrentDateTimeWithMilliseconds() {
        // Get the current time as a time_point
        auto now = std::chrono::system_clock::now();

        // Convert to a time_t for formatting
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to milliseconds since the epoch
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // Format the time to include date and time
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        
        // Add milliseconds to the formatted string
        oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

        return oss.str();
    }
}
