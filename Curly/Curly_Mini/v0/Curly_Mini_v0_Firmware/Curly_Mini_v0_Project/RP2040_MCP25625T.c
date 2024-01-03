
#include "RP2040_MCP25625T.h"
// pico-sdk
#include "pico/stdlib.h"
#include "hardware/spi.h"



void MCP25625_Setup(spi_inst_t *spi, uint sck_pin, uint cs_pin, uint spi_rx, uint spi_tx){
    spi_init(spi, MCP25625_SPI_HZ);
    
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_pull_up(cs_pin);               // Pullup just in case
    gpio_put(cs_pin, 1);                // CS is active low

    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(spi_rx, GPIO_FUNC_SPI);
    gpio_set_function(spi_tx, GPIO_FUNC_SPI);

    spi_get_hw(spi)->SSPCR0 &= ~(0x000F);   // Clear data width
    spi_get_hw(spi)->SSPCR0 |= 0x0007;      // set to 8 bit txfr widths
}

// spi_write_read_blocking(spi_inst_t *spi, const uint8_t *src, uint8_t *dst, size_t len)
uint8_t MCP25625_Read_Blocking(spi_inst_t *spi, uint8_t address){
    uint8_t tval[3]; 
    tval[0] = CAN_CTRL_READ; 
    tval[1] = address;
    uint8_t rval[3];
    spi_write_read_blocking(spi, tval, rval, sizeof(tval));
    return rval[2];
}

uint8_t MCP25625_Read_RX_Buffer_Blocking(spi_inst_t *spi, uint8_t buff_num){
    uint8_t tval[2]; 
    tval[0] = CAN_READ_RX_BUFF | buff_num; 
    uint8_t rval[2];
    spi_write_read_blocking(spi, tval, rval, sizeof(tval));
    return rval[1];
}

// int spi_write_blocking(spi_inst_t *spi, const uint8_t *src, size_t len);
void MCP25625_Write_Blocking(spi_inst_t *spi, uint8_t address, uint8_t data){
    uint8_t tval[3];
    tval[0] = CAN_CTRL_WRITE;
    tval[1] = address;
    tval[2] = data;
    spi_write_blocking(spi, tval, sizeof(tval));
}

void MCP25625_Write_TX_Buffer_Blocking(spi_inst_t *spi, uint8_t buff_num, uint8_t data){
    uint8_t tval[2];
    tval[0] = CAN_LOAD_TX_BUFF | buff_num;
    tval[1] = data;
    spi_write_blocking(spi, tval, sizeof(tval));
}

void MCP25625_RTS_Blocking(spi_inst_t *spi, uint8_t buff_num){
    uint8_t tval = CAN_CTRL_RTS | buff_num;
    spi_write_blocking(spi, tval, sizeof(tval));
}

void MCP25625_Bit_Modify_Blocking(spi_inst_t *spi, uint8_t addr, uint8_t mask, uint8_t data){
    uint8_t tval[4];
    tval[0] = CAN_CTRL_WRITE;
    tval[1] = addr;
    tval[2] = mask;
    tval[3] = data;
    spi_write_blocking(spi, tval, sizeof(tval));
}

uint8_t MCP25625_Read_Status_Blocking(spi_inst_t *spi){
    uint8_t tval[3]; 
    tval[0] = CAN_READ_STATUS; 
    uint8_t rval[3];
    spi_write_read_blocking(spi, tval, rval, sizeof(tval));
    return rval[2]; // bytes [1] and [2] are the same
}

uint8_t MCP25625_Read_RX_Buff_Status_Blocking(spi_inst_t *spi){
    uint8_t tval[3]; 
    tval[0] = CAN_RX_BUFF_STATUS; 
    uint8_t rval[3];
    spi_write_read_blocking(spi, tval, rval, sizeof(tval));
    return rval[2]; // bytes [1] and [2] are the same
}