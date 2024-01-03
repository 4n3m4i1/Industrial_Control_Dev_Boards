#ifndef RP2040_MCP25625_h
#define RP2040_MCP25625_h

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <inttypes.h>

// MCP25625 SPI Interface
/*
    CS Low -> {INSTRUCTION / COMMAND BYTE}
    Another command can't be set until CS is raised.

    Mode 0,0 and Mode 1,1 Supported
        (Data ingested on RISING, Data shifted on FALLING)
*/

// 10 MHz
#define MCP25625_SPI_HZ         (10 * 1000 * 1000)

// MCP25625 SPI Interface Commands
//  Datasheet Section 5 (pg 55)
#define CAN_CTRL_RESET          0xC0    // Equivalent to pin RESET
#define CAN_CTRL_READ           0x03
#define CAN_CTRL_WRITE          0x02
#define CAN_CTRL_RTS            0x80    // Set bits 0,1,2 for which TXB to send
#define CAN_BIT_MODIFY          0x05

// Quick Status Read, Controller/System Status
#define CAN_READ_STATUS         0xA0
enum CAN_READ_STATUS_BITS {
    RX0IF,
    RX1IF,
    TXREQ,
    TX0IF,
    TXREQ,
    TX1IF,
    TXREQ,
    TX2IF
};

// RX Information status
#define CAN_RX_BUFF_STATUS      0xB0
    // Masks for RX Status return
#define RXBUFSTAT_RX_MSG_OFFSET     6
#define RXBUFSTAT_RX_MSG_MASK       ((1 << 7) | (1 << 6))
enum RXBUFSTAT_RX_MSG_FIELD {
    RXBUFSTAT_NO_MESSAGE,
    RXBUFSTAT_MSG_IN_RXB0       = (1 << RXBUFSTAT_RX_MSG_OFFSET),
    RXBUFSTAT_MSG_IN_RXB1       = (2 << RXBUFSTAT_RX_MSG_OFFSET),
    RXBUFSTAT_MSG_IN_BOTH       = (3 << RXBUFSTAT_RX_MSG_OFFSET)
};

#define RXBUFSTAT_MSG_TYPE_OFFSET   3
#define RXBUFSTAT_MSG_TYPE_MASK     ((1 << 4) | (1 << 3))
enum RXBUFSTAT_MSG_TYPE_FIELD {
    RXBUFSTAT_STD_DATA_FRAME,
    RXBUFSTAT_STD_REMOTE_FRAME  = (1 << RXBUFSTAT_MSG_TYPE_OFFSET),
    RXBUFSTAT_EXT_DATA_FRAME    = (2 << RXBUFSTAT_MSG_TYPE_OFFSET),
    RXBUFSTAT_EXT_REMOTE_FRAME  = (3 << RXBUFSTAT_MSG_TYPE_OFFSET)
};

#define RXBUFSTAT_FILT_MATCH_OFFSET 0
#define RXBUFSTAT_FILT_MATCH_MASK   ((1 << 2) | (1 << 1) | (1 << 0))
enum RXBUFSTAT_FILT_MATCH_FIELD {
    MATCH_RXF0,
    MATCH_RXF1,
    MATCH_RXF2,
    MATCH_RXF3,
    MATCH_RXF4,
    MATCH_RXF5,
    MATCH_RXF0_ROLLOVER,
    MATCH_RXF1_ROLLOVER
};

// Quick Message Instructions
#define CAN_LOAD_TX_BUFF        0x40    // Set low 3 bits (0 - 5) to directly load TXB
#define CAN_READ_RX_BUFF        0x90    // Set bits 1,2 for which RXB to read


struct CAN_Message {
    uint32_t    id;     // eye dee
    uint8_t     ext;    // Is extended ID?
    uint8_t     len;    // Data field len in bytes, 0-8
    uint8_t     rtr;    // Is message a Remote Transmission RQ
    uint8_t     data[8];
};


#endif