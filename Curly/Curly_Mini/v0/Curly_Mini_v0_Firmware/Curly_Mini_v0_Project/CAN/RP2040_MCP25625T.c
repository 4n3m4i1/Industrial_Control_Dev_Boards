
#include "RP2040_MCP25625T.h"
// pico-sdk
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"



void MCP25625_Setup(spi_inst_t *spi, uint sck_pin, uint cs_pin, uint spi_rx, uint spi_tx, uint can_baud, uint can_clk_pin){
    spi_init(spi, MCP25625_SPI_HZ);     // 10MHz
    
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_pull_up(cs_pin);               // Pullup just in case
    gpio_put(cs_pin, true);                // CS is active low

    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(spi_rx, GPIO_FUNC_SPI);
    gpio_set_function(spi_tx, GPIO_FUNC_SPI);

    spi_get_hw(spi)->SSPCR0 &= ~(0x000F);   // Clear data width
    spi_get_hw(spi)->SSPCR0 |= 0x0007;      // set to 8 bit txfr widths

    // Setup other CAN GPIO stuff for ISRs and external reset.
    //  Much of this functionality can be done with the SPI interface,
    //      and will be removed in future revisions
#ifdef CURLY_MINI_v0
    gpio_init(CAN_RESET);
    gpio_set_dir(CAN_RESET, GPIO_OUT);
    gpio_put(CAN_RESET, true);

    gpio_init(CAN_TX2_RTS);
    gpio_set_dir(CAN_TX2_RTS, GPIO_OUT);
    gpio_put(CAN_TX2_RTS, true);
    gpio_init(CAN_TX1_RTS);
    gpio_set_dir(CAN_TX1_RTS, GPIO_OUT);
    gpio_put(CAN_TX1_RTS, true);
    gpio_init(CAN_TX0_RTS);
    gpio_set_dir(CAN_TX0_RTS, GPIO_OUT);
    gpio_put(CAN_TX0_RTS, true);

    gpio_init(CAN_RX_INT);
    gpio_set_dir(CAN_RX_INT, GPIO_IN);
    gpio_pull_up(CAN_RX_INT);

    gpio_init(CAN_RX_BUF0);
    gpio_set_dir(CAN_RX_BUF0, GPIO_IN);
    gpio_pull_up(CAN_RX_BUF0);

    gpio_init(CAN_RX_BUF1);
    gpio_set_dir(CAN_RX_BUF1, GPIO_IN);
    gpio_pull_up(CAN_RX_BUF1);
#endif
    gpio_put(CAN_RESET, false);

    gpio_init(can_clk_pin);
    gpio_set_dir(can_clk_pin, GPIO_OUT);
    // Setup CAN Clk @ 20MHz
    //  RP2040 base clock assumed to be 125MHz
    //  125M / 20M = 6.25
    //      Int = 6
    //      Frac is 4 bit fixed pt: .5, .25, .125, .0625
    //      Frac = 1 << 2
    // BUT divide by 2 since we need to run PWM, so:
    //      Int = 3
    //      Frac = .125 = (1 << 1)
    gpio_set_function(can_clk_pin, GPIO_FUNC_PWM);
    uint sliceno = pwm_gpio_to_slice_num(can_clk_pin);
    uint pwmchan = pwm_gpio_to_channel(can_clk_pin);
    pwm_set_chan_level(sliceno, pwmchan, 1);
    pwm_set_wrap(sliceno, 2);
    pwm_set_clkdiv_int_frac(sliceno, 3, (1 << 1));
    pwm_set_enabled(sliceno, true);

    // Finish boot reset
    gpio_put(CAN_RESET, true);

    // Now we need to setup the CAN bit clock... ew
}

// spi_write_read_blocking(spi_inst_t *spi, const uint8_t *src, uint8_t *dst, size_t len)
uint8_t MCP25625_Read_Blocking(spi_inst_t *spi ,uint8_t address){
    uint8_t tval[3]; 
    tval[0] = CAN_CTRL_READ; 
    tval[1] = address;
    uint8_t rval[3];
    spi_write_read_blocking(spi, tval, rval, sizeof(tval));
    return rval[2];
}

void MCP25625_CAN_Message_2_Raw_Arr(uint8_t *arr, struct CAN_Message *msg){
    if(msg && arr){
        arr[XBnDLC] = (msg->len & 0x0F);   // Set len
        arr[XBnSIDH] = ((msg->id >> 3) & 0x0F);
        arr[XBnSIDL] = ((msg->id & 0x07) << 5);
        if(msg->flags.ext){
            arr[XBnSIDL] |= (1 << 3);   // set extended
            arr[XBnSIDL] |= ((msg->id >> (16 + STDIDLEN)) & 0x03);
            arr[XBnEID8] =  (msg->id >> (8 + STDIDLEN));
            arr[XBnEID0] =  (msg->id >> (0 + STDIDLEN));
            arr[XBnDLC]  |= ((msg->flags.ext) ? (1 << 6) : 0);  // ext rtr
        } else {
            arr[XBnSIDL] |= (((msg->flags.remote) ? 1 : 0) << 4);
        }

        for(int n = 0; n < msg->len; ++n) arr[XBnD0 + n] = msg->data[n];
    }
}

void MCP25625_Raw_Arr_2_CAN_Message(struct CAN_Message *msg, uint8_t *arr){
    if(msg && arr){
        msg->id = (arr[XBnSIDL] >> 5) | arr[XBnSIDH];
        msg->flags.ext = arr[XBnSIDL] & (1 << 3);
        if(msg->flags.ext){
            msg->id |= (arr[XBnSIDL] & 0x03) << (16 + STDIDLEN);
            msg->id |= (arr[XBnEID8] << (8 + STDIDLEN));
            msg->id |= (arr[XBnEID0] << (0 + STDIDLEN));

            msg->flags.remote = arr[XBnDLC] & (1 << 6);
        } else {
            msg->flags.remote = arr[XBnSIDL] & (1 << 4);
        }

        msg->len = (arr[XBnDLC] & 0xF);

        for(int n = 0; n < msg->len; ++n) msg->data[n] = arr[XBnD0 + n]; 
    }
}

uint8_t MCP25625_Read_RX_Buffer_Blocking(spi_inst_t *spi, struct CAN_Message *msg, uint8_t buff_num){
    uint8_t tval[13]; 
    tval[0] = CAN_READ_RX_BUFF | buff_num; 
    uint8_t rval[13];
    // Starting transfer from top of RXBxSIDH -> 13 bytes total read
    //  Starting transfer from top of data field -> 8 bytes total read
    //  13 - 8 = 5
    if(buff_num & 0x01){    // Full read
        spi_write_read_blocking(spi, tval, rval, sizeof(tval));
        MCP25625_Raw_Arr_2_CAN_Message(msg, rval);
    } else {                // Just Data Read
        spi_write_read_blocking(spi, tval, rval, sizeof(tval) - MCP_MAILBOX_D0_OFFSET);
        for(int n = 0; n < 8; ++n) msg->data[n] = rval[n];
    }
    
    
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