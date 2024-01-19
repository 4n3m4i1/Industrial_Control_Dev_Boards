#ifndef CURLY_CAN_h
#define CURLY_CAN_h

#include "./CAN/RP2040_MCP25625T.h"
#include "Curly_Mini_Pins.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"



int Curly_CAN_Setup(uint BAUD);
int Curly_CAN_Pin_ISR();



#endif