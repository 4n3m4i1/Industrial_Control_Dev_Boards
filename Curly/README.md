# Curly
The Curly series is based on the RP2040, which needs no introduction. The simplicity and open nature of the toolchain make this platform ideal in many respects, and the PIO can be leveraged heavily for custom peripheral creation.  
  
[Datasheet/Reference Manual](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
  
By default a single size of the Curly is offered.
  
## Curly Sizes
- Curly Mini
    - RP2040, 3V3 I/O
    - 44.5mm x 20mm x 1.6mm, 4 layer
    - Single side pinout for vertical mounting in PCB slot
    - 8 GPIO, 4 A2D / GPIO (Total 12) (AREF == 3.3V, not precision ref)
    - External Chip Reset Pin (Active Low)
    - 7V - 48V DC Input Voltage
    - Integrated CAN controller and transceiver (MCP25625T)
    - Integrated 2 channel RS232 converter (MAX3232)
    - NOT breadboard compatible, designed for custom vertical mounting in DIN enclosures
    - Custom 24 pin footprint + symbol provided in library
    - 3 pin Pico/ARM debug and programming interface
    - 3 pin USB 2.0 interface (D+, D-, GND) on header
    - For JLC/LCSC manufacturing the MCP25625T is used as it's more available