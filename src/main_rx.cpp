#include <iostream>
#include <mqueue.h>
#include <cstring>
#include "ipc_data.pb.h"

#define QUEUE_NAME "/ipc_queue"
#define MAX_MESSAGE_SIZE 1024
#define QUEUE_PERMISSIONS 0660

/**
 * @brief Safely print a Protobuf IPCData object in a human-readable format.
 * 
 * @param data The IPCData object to print.
 */
void printData(const IPCData& data) {
    std::cout << "int: " << (data.has_the_int() ? std::to_string(data.the_int()) : "N/A")
              << ", float: " << (data.has_the_float() ? std::to_string(data.the_float()) : "N/A")
              << ", string: " << (data.has_the_string() ? data.the_string() : "N/A")
              << ", enum: " << (data.has_the_type() ? std::to_string(data.the_type()) : "N/A")
              << std::endl;
}

int main() {
    // Remove any leftover queue from previous runs
    mq_unlink(QUEUE_NAME);

    // Set up message queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;               // Blocking mode
    attr.mq_maxmsg = 10;             // Max number of messages in queue
    attr.mq_msgsize = MAX_MESSAGE_SIZE; // Max size of a single message
    attr.mq_curmsgs = 0;             // Number of messages currently in the queue

    // Open the message queue
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, QUEUE_PERMISSIONS, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    std::cout << "Waiting for messages..." << std::endl;

    char buffer[MAX_MESSAGE_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MESSAGE_SIZE, nullptr);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }

        // Deserialize the message
        IPCData data;
        if (!data.ParseFromArray(buffer, bytes_read)) {
            std::cerr << "Failed to parse message" << std::endl;
            continue;
        }

        // Print the received message
        std::cout << "Received Message:" << std::endl;
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
    }

    // Close and unlink the message queue
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}