#include <iostream>
#include <mqueue.h>
#include <csignal>
#include <cstring>
#include <unistd.h> // For usleep
#include "t_ipc_data.h"
#include "constants.h"
#include "util.h"

/**
 * @brief Signal handler for Ctrl+C (SIGINT).
 * 
 * Sets the `stop` flag to gracefully exit the main loop when Ctrl+C is pressed.
 * 
 * @param signal The signal number (SIGINT).
 */
volatile sig_atomic_t stop = 0;
void handle_signal(int signal) {
    stop = 1; // Set stop flag when receiving SIGINT
}

/**
 * @brief Displays a spinner animation in the console.
 * 
 * This method updates a spinner animation in the console every 100 ms
 * while waiting for messages. It uses terminal control sequences to
 * update the spinner in place.
 */
void show_dots_spinner() {
    const std::string dots[] = {"⠙", "⠸", "⠴", "⠦", "⠧", "⠇", "⠋"};
    static int position = 0;

    std::cout << "\r\r\033[32m" << dots[position++] << " \033[0m Waiting for messages... Ctrl+C to stop." << std::flush;
    position %= sizeof(dots) / sizeof(dots[0]);

    usleep(100000); // 100 ms delay
}

/**
 * @brief Processes a single message received from the queue.
 * 
 * Deserializes the message into a `T_IPCData` object and prints its content.
 * If deserialization fails, logs the error.
 * 
 * @param buffer The raw message buffer received from the queue.
 * @param size The size of the received message in bytes.
 */
void process_message(const char* buffer, ssize_t size) {
    std::cout << std::endl; // Ensure output starts on a new line
    std::string serializedMessage(buffer, size);

    try {
        // Deserialize message into T_IPCData
        T_IPCData data(serializedMessage);
        // Print the deserialized message
        std::cout << "Received Message @ " << util::getCurrentDateTimeWithMilliseconds() << ":\n" << data.ToString() << std::endl;
    } catch (const std::exception& e) {
        // Print any errors encountered during processing
        std::cerr << "Error processing message: " << e.what() << std::endl;
    }
}

int main() {
    // Catch SIGINT (Ctrl+C) for graceful shutdown
    signal(SIGINT, handle_signal);

    // Remove any leftover queue from previous runs
    mq_unlink(QUEUE_NAME);

    // Set up message queue attributes
    struct mq_attr attr;
    attr.mq_flags = O_NONBLOCK; // Non-blocking mode
    attr.mq_maxmsg = 10;        // Max number of messages in queue
    attr.mq_msgsize = MAX_MESSAGE_SIZE; // Max size of a single message
    attr.mq_curmsgs = 0;        // Number of messages currently in the queue

    // Open the message queue
    // O_CREAT == Create the queue if it doesn't already exist
    // O_RDONLY == Read only
    // O_NONBLOCK == Open the queue in non-blocking mode (for mq_receive below)
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, QUEUE_PERMISSIONS, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    char buffer[MAX_MESSAGE_SIZE];

    // Wait for messages
    while (!stop) {
        show_dots_spinner();

        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MESSAGE_SIZE, nullptr);

        /*
            >= 0 rather than > 0 because a message with empty values is STILL a valid message,
            but will come over blank.
        */
        if (bytes_read >= 0) {
            process_message(buffer, bytes_read);
        } else if (bytes_read == -1 && errno != EAGAIN) {
            perror("\nmq_receive");
            break; // Fail hard on unexpected mq_receive error
        }
    }

    std::cout << "\nExiting..." << std::endl;

    // Close and unlink the message queue
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
