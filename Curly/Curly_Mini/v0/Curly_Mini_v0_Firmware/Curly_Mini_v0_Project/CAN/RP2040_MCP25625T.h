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
// lower 3 bits
enum CANLOADTX_A_B_C {
    TXB0SIDH,           // 0x31
    TXB0D0,             // 0x36
    TXB1SIDH,           // 0x41
    TXB1D0,             // 0x46
    TXB2SIDH,           // 0x51
    TXB2D0
};

#define CAN_READ_RX_BUFF        0x90    // Set bits 1,2 for which RXB to read
// bits 1,2 or n,m. Where to start reading the buffer from
enum CANREADRX_N_M {
    RXB0SIDH,           // 0x61
    RXB0D0,             // 0x66
    RXB1SIDH,           // 0x71
    RXB1D0              // 0x76
};

struct CAN_Message {
    uint32_t    id;         // eye dee
    struct {
        uint8_t ext;        // Extended ID?
        uint8_t remote;     // Remote RX, or queued as RTR TX?
    } flags;
    uint8_t     len;        // Data field len in bytes, 0-8
    uint8_t     mbno;       // Mailbox Number
    uint8_t     data[8];    // Data
};


#define STDIDLEN            11
#define EXTIDLEN            29
#define EXT_ID_OFFSET       (EXTIDLEN - STDIDLEN)

#define MCP_MAILBOX_D0_OFFSET   5
enum MCP_Mailbox_Raw {
    XBnSIDH,
    XBnSIDL,
    XBnEID8,
    XBnEID0,
    XBnDLC,
    XBnD0,
    XBnD1,
    XBnD2,
    XBnD3,
    XBnD4,
    XBnD5,
    XBnD6,
    XBnD7
};




// CANCTRL and CANSTAT have many aliases

enum MCP25625_MEM_MAP {
    MMAP_RXF0SIDH,
    MMAP_RXF0SIDL,
    MMAP_RXF0EID8,
    MMAP_RXF0EID0,
    MMAP_RXF1SIDH,
    MMAP_RXF1SIDL,
    MMAP_RXF1EID8,
    MMAP_RXF1EID0,
    MMAP_RXF2SIDH,
    MMAP_RXF2SIDL,
    MMAP_RXF2EID8,
    MMAP_RXF2EID0,
    MMAP_BFPCTRL,
    MMAP_TXRTSCTRL,
    MMAP_CANSTAT_A0,
    MMAP_CANCTRL_A0,

    MMAP_RXF3SIDH,
    MMAP_RXF3SIDL,
    MMAP_RXF3EID8,
    MMAP_RXF3EID0,
    MMAP_RXF4SIDH,
    MMAP_RXF4SIDL,
    MMAP_RXF4EID8,
    MMAP_RXF4EID0,
    MMAP_RXF5SIDH,
    MMAP_RXF5SIDL,
    MMAP_RXF5EID8,
    MMAP_RXF5EID0,
    MMAP_TEC,
    MMAP_REC,
    MMAP_CANSTAT_A1,
    MMAP_CANCTRL_A1,

    MMAP_RXM0SIDH,
    MMAP_RXM0SIDL,
    MMAP_RXM0EID8,
    MMAP_RXM0EID0,
    MMAP_RXM1SIDH,
    MMAP_RXM1SIDL,
    MMAP_RXM1EID8,
    MMAP_RXM1EID0,
    MMAP_CNF3,
    MMAP_CNF2,
    MMAP_CNF1,
    MMAP_CANINTE,
    MMAP_CANINTF,
    MMAP_EFLG,
    MMAP_CANSTAT_A2,
    MMAP_CANCTRL_A2,

    MMAP_TXB0CTRL,
    MMAP_TXB0SIDH,
    MMAP_TXB0SIDL,
    MMAP_TXB0EID8,
    MMAP_TXB0EID0,
    MMAP_TXB0DLC,
    MMAP_TXB0D0,
    MMAP_TXB0D1,
    MMAP_TXB0D2,
    MMAP_TXB0D3,
    MMAP_TXB0D4,
    MMAP_TXB0D5,
    MMAP_TXB0D6,
    MMAP_TXB0D7,
    MMAP_CANSTAT_A3,
    MMAP_CANCTRL_A3,

    MMAP_TXB1CTRL,
    MMAP_TXB1SIDH,
    MMAP_TXB1SIDL,
    MMAP_TXB1EID8,
    MMAP_TXB1EID0,
    MMAP_TXB1DLC,
    MMAP_TXB1D0,
    MMAP_TXB1D1,
    MMAP_TXB1D2,
    MMAP_TXB1D3,
    MMAP_TXB1D4,
    MMAP_TXB1D5,
    MMAP_TXB1D6,
    MMAP_TXB1D7,
    MMAP_CANSTAT_A4,
    MMAP_CANCTRL_A4,

    MMAP_TXB2CTRL,
    MMAP_TXB2SIDH,
    MMAP_TXB2SIDL,
    MMAP_TXB2EID8,
    MMAP_TXB2EID0,
    MMAP_TXB2DLC,
    MMAP_TXB2D0,
    MMAP_TXB2D1,
    MMAP_TXB2D2,
    MMAP_TXB2D3,
    MMAP_TXB2D4,
    MMAP_TXB2D5,
    MMAP_TXB2D6,
    MMAP_TXB2D7,
    MMAP_CANSTAT_A5,
    MMAP_CANCTRL_A5,
    
    MMAP_RXB0CTRL,
    MMAP_RXB0SIDH,
    MMAP_RXB0SIDL,
    MMAP_RXB0EID8,
    MMAP_RXB0EID0,
    MMAP_RXB0DLC,
    MMAP_RXB0D0,
    MMAP_RXB0D1,
    MMAP_RXB0D2,
    MMAP_RXB0D3,
    MMAP_RXB0D4,
    MMAP_RXB0D5,
    MMAP_RXB0D6,
    MMAP_RXB0D7,
    MMAP_CANSTAT_A6,
    MMAP_CANCTRL_A6,

    MMAP_RXB1CTRL,
    MMAP_RXB1SIDH,
    MMAP_RXB1SIDL,
    MMAP_RXB1EID8,
    MMAP_RXB1EID0,
    MMAP_RXB1DLC,
    MMAP_RXB1D0,
    MMAP_RXB1D1,
    MMAP_RXB1D2,
    MMAP_RXB1D3,
    MMAP_RXB1D4,
    MMAP_RXB1D5,
    MMAP_RXB1D6,
    MMAP_RXB1D7,
    MMAP_CANSTAT_A7,
    MMAP_CANCTRL_A7
};


#endif