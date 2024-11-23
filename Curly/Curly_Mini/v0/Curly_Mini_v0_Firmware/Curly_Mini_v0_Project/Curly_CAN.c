#include "Curly_CAN.h"
#include "Curly_Mini_Pins.h"


int Curly_CAN_Setup(struct Curly_CAN_Status *stat, uint BAUD){
    int retval = 0;
    if(stat){
        // spinum, sck, cs, rx, tx, canbaud, can clk pin
        MCP25625_Setup(SPI_CAN, CAN_SPI_SCK, CAN_SPI_CS, CAN_SPI_RX, CAN_SPI_TX, DFL_CANBAUD, MCP25625_CLK);
        stat->rxmb[0] = CCAN_DFL;
        stat->rxmb[1] = CCAN_DFL;

        stat->txmb[0] = CCAN_DFL;
        stat->txmb[1] = CCAN_DFL;
        stat->txmb[2] = CCAN_DFL;
        retval = 1;
    }
    return retval;
}

int Curly_CAN_Pin_ISR_Setup(struct Curly_CAN_Status *stat, bool isr_on);
void Curly_CAN_Pin_ISR_Handler();


int Curly_CAN_Setup_Remote(struct CAN_Message *stat, struct CAN_Message *msg, int txmb_no);
int Curly_CAN_Update_Remote(struct CAN_Message *stat, struct CAN_Message *msg, int txmb_no);

int Curly_CAN_TX_Message(struct CAN_Message *stat, struct CAN_Message msg, int txmb_no);