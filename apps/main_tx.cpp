#include <iostream>
#include <random>
#include <csignal>
#include <thread>
#include <chrono>
#include "util.h"
#include "t_ipc_data.h"
#include "../shared/constants.h"
#include <mqueue.h> // POSIX message queue
#include <iomanip>
#include <sstream>

/**
 * @brief Signal handler for Ctrl+C (SIGINT).
 * 
 * Sets the `stop` flag to gracefully exit the main loop when Ctrl+C is pressed.
 * 
 * @param signal The signal number (SIGINT).
 */
volatile sig_atomic_t stop = 0;
void handle_signal(int signal) {
    stop = 1;
}

/**
 * @brief Displays a spinner animation in the console.
 * 
 * This method updates a spinner animation in the console while generating data.
 * The spinner cycles through a sequence of characters to indicate progress.
 */
void show_dots_spinner() {
    const std::string dots[] = {"⠙", "⠸", "⠴", "⠦", "⠧", "⠇", "⠋"};
    static int position = 0;
    std::cout << "\r\r\033[95m" << dots[position++] << " \033[0m Generating data... Ctrl+C to stop." << std::flush;
    position %= 7;
}

/**
 * @brief Generates a random T_IPCData object.
 * 
 * Creates a random T_IPCData object with fields set based on probabilities:
 * - 5% chance no fields are set.
 * - 20% chance one field is set.
 * - 25% chance three fields are set.
 * - 50% chance all fields are set.
 * 
 * @return A randomly populated T_IPCData object.
 */
T_IPCData generate_random_data() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> int_dist(0, 100);
    static std::uniform_real_distribution<float> float_dist(0.0f, 100.0f);
    static std::uniform_int_distribution<int> enum_dist(0, 2);
    static std::uniform_int_distribution<int> chance(0, 99);

    // Determine how many fields are set based on probability
    int random_chance = chance(gen);
    if (random_chance < 5) { // 5% chance no values are set
        return T_IPCData{};
    } else if (random_chance < 25) { // 20% chance one value is set
        int field_to_set = chance(gen) % 4;
        if (field_to_set == 0) return T_IPCData{int_dist(gen), std::nullopt, std::nullopt, std::nullopt};
        if (field_to_set == 1) return T_IPCData{std::nullopt, float_dist(gen), std::nullopt, std::nullopt};
        if (field_to_set == 2) return T_IPCData{std::nullopt, std::nullopt, "RandomString", std::nullopt};
        return T_IPCData{std::nullopt, std::nullopt, std::nullopt, static_cast<IPCData::Type>(enum_dist(gen))};
    } else if (random_chance < 50) { // 25% chance 3/4 values are set
        return T_IPCData{
            int_dist(gen), 
            float_dist(gen), 
            util::getCurrentDateTimeWithMilliseconds(), 
            std::nullopt
        };
    } else { // 50% chance all values are set
        return T_IPCData{
            int_dist(gen), 
            float_dist(gen), 
            util::getCurrentDateTimeWithMilliseconds(), 
            static_cast<IPCData::Type>(enum_dist(gen))
        };
    }
}

/**
 * @brief Sends a message to the POSIX message queue.
 * 
 * Serializes a `T_IPCData` object and sends it to the designated message queue.
 * If the queue is full or inaccessible, an exception is thrown.
 * 
 * @param data A `T_IPCData` object containing the data to be sent.
 * @throws std::runtime_error if the message queue cannot be opened or the message cannot be sent.
 */
void txMessage(const T_IPCData& data) {
    // Open the message queue
    // O_WRONLY == Write only
    // O_CREAT == Create the queue if it doesn't already exist
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, QUEUE_PERMISSIONS, nullptr);
    if (mq == (mqd_t)-1) {
        throw std::runtime_error("Failed to open message queue");
    }

    // Serialize the message
    std::string serializedMessage = data.Serialize();

    // Send the serialized message
    if (mq_send(mq, serializedMessage.c_str(), serializedMessage.size(), 0) == -1) {
        mq_close(mq);
        throw std::runtime_error("Failed to send message");
    }

    mq_close(mq);
}

int main() {
    std::signal(SIGINT, handle_signal);

    std::cout << "Starting Tx process. Press Ctrl+C to stop.\n";

    while (!stop) {
        T_IPCData data = generate_random_data();

        try {
            txMessage(data);
            std::cout << "\nMessage Sent:\n" << data.ToString();
        } catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }

        for (int i = 0; i < 10 && !stop; ++i) {
            show_dots_spinner();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    std::cout << "\nTx process terminated.\n";
    return 0;
}
