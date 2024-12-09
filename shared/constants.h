#ifndef CONSTANTS_H
#define CONSTANTS_H

// Name of the POSIX message queue
// This name is used by both the sender and receiver to identify the queue.
// The forward slash ('/') is required as a prefix for POSIX message queue names.
#define QUEUE_NAME "/ipc_queue"

// Maximum size of messages in the queue
// This defines the maximum size (in bytes) of a single message that can be sent or received.
#define MAX_MESSAGE_SIZE 1024

// Permissions for the message queue
// These are UNIX file system-style permissions (octal format) for the queue:
// - 0660: Read and write permissions for the owner and group, but no access for others.
#define QUEUE_PERMISSIONS 0660

#endif // CONSTANTS_H