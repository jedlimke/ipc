#include <iostream>
#include <mqueue.h>
#include <csignal>
#include <cstring>
#include <unistd.h> // For usleep
#include "t_ipc_data.h"
#include "constants.h"

// Signal handler for Ctrl+C
volatile sig_atomic_t stop = 0;
void handle_signal(int signal) {
    stop = 1; // Set stop flag when receiving SIGINT
}

// Spinner animation
void show_dots_spinner() {
    const std::string dots[] = {"⠙", "⠸", "⠴", "⠦", "⠧", "⠇", "⠋"};
    static int position = 0;

    std::cout << "\r\033[32m" << dots[position++] << " \033[0m Waiting for messages... Ctrl+C to stop." << std::flush;
    position %= sizeof(dots) / sizeof(dots[0]);

    usleep(100000); // 100 ms delay
}

// Process a single message from the queue
void process_message(const char* buffer, ssize_t size) {
    std::string serializedMessage(buffer, size);
    try {
        T_IPCData data(serializedMessage); // Deserialize directly using the constructor
        std::cout << "Received Message:\n" << data.ToString();
    } catch (const std::exception& e) {
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
        if (bytes_read > 0) {
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
