#ifndef CURLY_MINI_PINS_h
#define CURLY_MINI_PINS_h

// Pin name -> gpio number
enum WAFFLE_MINI_v0_PINS {
    UART0_TX,           // UART0 Hardware Peripheral, tied to MAX3232
    UART0_RX,
    GPIO_2,
    GPIO_3,
    UART1_TX,           // UART1 Hardware Peripheral, tied to MAX3232
    UART1_RX,
    GPIO_6,
    GPIO_7,
    CAN_TX2_RTS,
    CAN_TX1_RTS,
    CAN_TX0_RTS,
    CAN_RESET,
    CAN_SPI_RX,         // SPI1 Hardware Periperal
    CAN_SPI_CS,
    CAN_SPI_SCK,
    CAN_SPI_TX,
    CAN_RX_INT,         // RX ISR Input
    CAN_RX_BUF0,
    CAN_RX_BUF1,
    MCP25625_CLK,       // Clock output to can controller
    CAN_TXVR_STBY,
    BOARD_LED,
    GPIO_22,
    GPIO_23,
    GPIO_24,
    GPIO_25,
    GPIO_26_A0,
    GPIO_27_A1,
    GPIO_28_A2,
    GPIO_29_A3
};

#define SPI_CAN     spi1

#endif