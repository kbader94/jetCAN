#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include <stdbool.h>

/* Message Framing */
#define MESSAGE_START 0xAA
#define MESSAGE_END   0x55

/* Message Types */
#define MESSAGE_TYPE_COMMAND  0x01
#define MESSAGE_TYPE_DATA     0x02

/* Payload Constraints */
#define MAX_PAYLOAD_SIZE 32  /* Adjust based on RAM availability */

/* Command Definitions */
#define COMMAND_RECORD_REQ_START  0xF000
#define COMMAND_RECORD_STARTED    0xF001
#define COMMAND_RECORD_REQ_END    0xE000
#define COMMAND_RECORD_ENDED      0xE001
#define COMMAND_SHUTDOWN_REQ      0xD000
#define COMMAND_SHUTDOWN_STARTED  0xD001

/* Baud Rate Definitions */
#ifdef ARDUINO
    #define ARDUINO_BAUDRATE 9600
    #define GET_TIME_MS() millis()
#else
    #define LINUX_BAUDRATE B9600
    #include <time.h>
    static inline uint32_t GET_TIME_MS(void) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    }
#endif

/* Timeout for message reception (milliseconds) */
#define MAX_MESSAGE_TIMEOUT_MS 100

/* Command Structure */
struct Command {
    uint8_t agent;
    uint16_t command;
};

/* Status Structure */
struct Status {
    uint8_t battery_level;
    uint8_t battery_voltage;
    bool recording;
    bool lights_on;
};

/* Message Structure */
struct Message {
    uint8_t recipient;
    uint8_t message_type; /* command or data */
    uint8_t payload_length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
};

/* Initialize communication */
void comms_init(void);

/* Send a message */
void comms_send_message(uint8_t recipient, uint8_t type, uint8_t *payload, uint8_t length);

/* Receive messages (non-blocking on Arduino) */
bool comms_receive_message(struct Message *msg);

/* Utility function to calculate checksum */
uint8_t comms_calculate_checksum(uint8_t *data, uint8_t length);

/* Close communication (for Linux) */
void comms_close(void);

#endif /* COMMS_H */
