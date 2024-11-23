#ifndef CURLY_CAN_h
#define CURLY_CAN_h

#include "./CAN/RP2040_MCP25625T.h"
#include "Curly_Mini_Pins.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

enum CURLY_CAN_STAT_FLAGS {
    CCAN_DFL,
    CCAN_TX_REMOTE,
    CCAN_TX_STD,
    CCAN_RX_REMOTE,
    CCAN_RX_STD
};

struct Curly_CAN_Status {
    uint8_t txmb[3];
    uint8_t rxmb[2];
};

int Curly_CAN_Setup(struct Curly_CAN_Status *stat, uint BAUD);
int Curly_CAN_Pin_ISR_Setup(struct Curly_CAN_Status *stat, bool isr_on);
void Curly_CAN_Pin_ISR_Handler();


int Curly_CAN_Setup_Remote(struct CAN_Message *stat, struct CAN_Message *msg, int txmb_no);
int Curly_CAN_Update_Remote(struct CAN_Message *stat, struct CAN_Message *msg, int txmb_no);

int Curly_CAN_TX_Message(struct CAN_Message *stat, struct CAN_Message msg, int txmb_no);



#endif