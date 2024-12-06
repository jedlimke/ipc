#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <ctime>
#include <unistd.h> // For usleep
#include "ipc_data.pb.h"

#define QUEUE_NAME "/ipc_queue"
#define MAX_MESSAGE_SIZE 1024
#define QUEUE_PERMISSIONS 0660

volatile sig_atomic_t stop = 0;

// Signal handler for Ctrl+C
void handle_signal(int signal) {
    stop = 1; // Set stop flag when receiving SIGINT
}

// Spinner animation
void show_spinner() {
    const std::string dots[] = {"⠙", "⠸", "⠴", "⠦", "⠧", "⠇", "⠋"};
    static int position = 0;

    std::cout << "\rWaiting for messages... " << dots[position++] << " " << std::flush;
    position %= sizeof(dots) / sizeof(dots[0]);

    usleep(100000); // 100 ms delay
}

int main() {
    // Catch SIGINT (Ctrl+C) for graceful shutdown
    signal(SIGINT, handle_signal);

    // Remove any leftover queue from previous runs
    mq_unlink(QUEUE_NAME);

    // Set up message queue attributes
    struct mq_attr attr;
    attr.mq_flags = O_NONBLOCK;      // Non-blocking mode
    attr.mq_maxmsg = 10;             // Max number of messages in queue
    attr.mq_msgsize = MAX_MESSAGE_SIZE; // Max size of a single message
    attr.mq_curmsgs = 0;             // Number of messages currently in the queue

    // Open the message queue
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, QUEUE_PERMISSIONS, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    std::cout << "Press Ctrl+C to stop.\n" << std::endl;

    char buffer[MAX_MESSAGE_SIZE];
    while (!stop) {
        // Show spinner while waiting for messages
        show_spinner();

        // Try to receive a message
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MESSAGE_SIZE, nullptr);

        if (bytes_read > 0) {
            // Clear spinner and print the received message
            std::cout << "\rReceived Message:                                   \n";

            // Deserialize the message
            IPCData data;
            if (!data.ParseFromArray(buffer, bytes_read)) {
                std::cerr << "Failed to parse message" << std::endl;
                continue;
            }

            // Print message details
            if (data.has_the_int()) {
                std::cout << "  Int: " << data.the_int() << std::endl;
            } else {
                std::cout << "  Int: Not set" << std::endl;
            }
            if (data.has_the_float()) {
                std::cout << "  Float: " << data.the_float() << std::endl;
            } else {
                std::cout << "  Float: Not set" << std::endl;
            }
            if (data.has_the_string()) {
                std::cout << "  String: " << data.the_string() << std::endl;
            } else {
                std::cout << "  String: Not set" << std::endl;
            }
            if (data.has_the_type()) {
                std::cout << "  Type: " << data.the_type() << std::endl;
            } else {
                std::cout << "  Type: Not set" << std::endl;
            }
        } else if (bytes_read == -1 && errno != EAGAIN) {
            perror("\nmq_receive");
        }
    }

    std::cout << "\nExiting..." << std::endl;

    // Close and unlink the message queue
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
